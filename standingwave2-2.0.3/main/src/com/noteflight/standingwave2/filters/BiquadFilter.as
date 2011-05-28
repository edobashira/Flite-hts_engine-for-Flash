////////////////////////////////////////////////////////////////////////////////
//
//  NOTEFLIGHT LLC
//  Copyright 2009 Noteflight LLC
// 
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////


package com.noteflight.standingwave2.filters
{
    import __AS3__.vec.Vector;
    
    import com.noteflight.standingwave2.elements.IAudioSource;
    
    /**
     * Infinite Impulse Response (IIR) linear filter based on the "Direct Form 1"
     * filter structure, incorporating four delay lines from the two previous input and
     * output values.
     *  
     * This filter can be used in three ways: as a low-pass filter that attenuates frequencies
     * higher than the <code>frequency</code> property, as a high-pass filter that attenuates frequencies lower
     * than the center, or as a band-pass filter that attenuates frequencies that lie
     * further from the center.  In all three cases the <code>resonance</code> property controls
     * the abruptness of the rolloff as a function of frequency.  The <code>type</code> property
     * determines which filter behavior is used.  
     */
    public class BiquadFilter extends AbstractFilter
    {
        private var _frequency:Number;
        private var _resonance:Number;
        private var _type:int;
        
        private var _state:Array = null;  // per-channel arrays of delay line values
        
        private var _calculated:Boolean = false; // true when coefficients have been calculated
        private var _a0:Number, _a1:Number, _a2:Number; 
        private var _b0:Number, _b1:Number, _b2:Number; 
        
        /** Low-pass filter type */
        public static const LOW_PASS_TYPE:int = 0;

        /** High-pass filter type */
        public static const HIGH_PASS_TYPE:int = 1;

        /** Band-pass filter type (constant peak, attenuated skirt) */
        public static const BAND_PASS_TYPE:int = 2;
        
        /**
         * Construct an instance of a BiquadFilter.  Parameters may be left as defaulted and/or changed
         * later while the filter is in operation.
         * 
         * @param source the underlying audio source
         * @param type the type of filter desired
         * @param frequency the center frequency of the filter
         * @param resonance the resonance characteristic of the filter, also known as "Q"
         */
        public function BiquadFilter(source:IAudioSource = null, type:int = LOW_PASS_TYPE, frequency:Number = 1000, resonance:Number = 1)
        {
            super(source);
            this.type = type;
            this.frequency = frequency;
            this.resonance = resonance;
        }

        override public function resetPosition():void
        {
            super.resetPosition();

            // Initialize delay line state per channel when the cursor is reset
            _state = [];
            for (var i:int = 0; i < descriptor.channels; i++)
            {
                _state[i] = [ 0, 0, 0, 0 ];
            }
        }        
        
        /**
         * The type of the filter, which controls the shape of its frequency response.
         */
        public function get type():int
        {
            return _type;
        }
        
        public function set type(value:int):void
        {
            _type = value;
            invalidateCoefficients();
        }
        
        /**
         * The center frequency of the filter in Hz. 
         */
        public function get frequency():Number
        {
            return _frequency;
        }
        
        public function set frequency(value:Number):void
        {
            _frequency = value;
            invalidateCoefficients();
        }
        
        /**
         * The resonance of the filter, which controls the degree of attenuation in its frequency response. 
         */
        public function get resonance():Number
        {
            return _resonance;
        }
        
        public function set resonance(value:Number):void
        {
            _resonance = value;
            invalidateCoefficients();
        }
        
        /**
         * Called when a property change invalidates the derived coefficients for the filter. 
         */
        protected function invalidateCoefficients():void
        {
            _calculated = false;
        }
        
        /**
         * Called to force calculation of the derived coefficients.
         */
        protected function computeCoefficients():void
        {
            if (_calculated)
            {
                return;
            }
            
            // Set up filter coefficients.
            // Formulae courtesy of http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
            //
            var w0:Number = 2 * Math.PI * _frequency / descriptor.rate;
            var cosw0:Number = Math.cos(w0);
            var sinw0:Number = Math.sin(w0);
            var alpha:Number = sinw0 / (2 * _resonance);
            
            switch (_type)
            {
                case LOW_PASS_TYPE:
                    _b0 = (1 - cosw0) / 2;
                    _b1 = 1 - cosw0;
                    _b2 = (1 - cosw0) / 2
                    _a0 = 1 + alpha;
                    _a1 = -2 * cosw0;
                    _a2 = 1 - alpha;
                    break;
                    
                case HIGH_PASS_TYPE:
                    _b0 = (1 + cosw0) / 2;
                    _b1 = -(1 + cosw0);
                    _b2 = (1 + cosw0) / 2;
                    _a0 = 1 + alpha;
                    _a1 = -2 * cosw0;
                    _a2 = 1 - alpha;
                    break;
                    
                case BAND_PASS_TYPE:
                    _b0 = alpha;
                    _b1 = 0;
                    _b2 = -alpha;
                    _a0 = 1 + alpha;
                    _a1 = -2 * cosw0;
                    _a2 = 1 - alpha;
                    break;
            }
            
            // Normalization step for coefficients other than a0
            _b0 /= _a0;
            _b1 /= _a0;
            _b2 /= _a0;
            _a1 /= _a0;
            _a2 /= _a0;
            
            _calculated = true;
        }
        
        ////////////////////////////////////////////        
        // overrideable abstract methods
        ////////////////////////////////////////////

        override protected function transformChannel(data:Vector.<Number>, channel:Number, start:Number, numFrames:Number):void
        {
            // Get our magic numbers.
            computeCoefficients();
            
            // Cache the delay line values for each channel in a set of locals for efficiency
            var channelState:Array = _state[channel];
            var x1:Number = channelState[0];
            var x2:Number = channelState[1];
            var y1:Number = channelState[2];
            var y2:Number = channelState[3];
            
            // Apply the linear filter for each sample frame, and cascade values through the
            // input and output delay lines.
            //
            for (var i:Number = 0; i < numFrames; i++)
            {
                var x:Number = data[i];
                var y:Number = x*_b0 + x1*_b1 + x2*_b2 - y1*_a1 - y2*_a2;
                x2 = x1;
                x1 = x;
                y2 = y1;
                data[i] = y1 = y;
            }

            // Put the locals back into the cached delay line state for this channel
            channelState[0] = x1;
            channelState[1] = x2;
            channelState[2] = y1;
            channelState[3] = y2;                 
        }

        override public function clone():IAudioSource
        {
            return new BiquadFilter(source.clone(), type, frequency, resonance);
        }
    }
}
