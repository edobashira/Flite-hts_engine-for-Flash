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
    import com.noteflight.standingwave2.elements.Sample;
    
    /**
     * An EchoFilter implements a simple recirculating delay line in which the input is blended with
     * a time-delayed copy of itself.  
     */
    public class EchoFilter extends AbstractFilter
    {
        private var _bufferLength:Number;
        private var _wet:Number;
        private var _decay:Number;
        private var _period:Number;
        private var _ring:Sample;
        
        /**
         * Create a new EchoFilter.  Parameters may be changed while the filter is operating.
         *  
         * @param source the underlying IAudioSource
         * @param period the time period of the echo
         * @param wet the fraction of the output which is represented by the delayed signal
         * @param decay the fraction of the delayed signal which is fed back into the delay line
         * 
         */
        public function EchoFilter(source:IAudioSource = null, period:Number = 0, wet:Number = 0.5, decay:Number = 0.5)
        {
            super(source);
            this.period = period;
            this.wet = wet;
            this.decay = decay;
        }
        
        /**
         * @inheritDoc
         */
        override public function resetPosition():void
        {
            super.resetPosition();
            initializeState();
        }        
        
        /**
         * The fraction of the output which is represented by the delayed signal.
         */
        public function get wet():Number
        {
            return _wet;
        }
        
        public function set wet(value:Number):void
        {
            _wet = value;
            initializeState();
        }
        
        /**
         * The fraction of the delayed signal which is fed back into the delay line.
         */
        public function get decay():Number
        {
            return _decay;
        }
        
        public function set decay(value:Number):void
        {
            _decay = value;
            initializeState();
        }
        
        /**
         * The time period of the echo in seconds.
         */
        public function get period():Number
        {
            return _period;
        }
        
        public function set period(value:Number):void
        {
            _period = value;
            initializeState();
        }
        
        protected function initializeState():void
        {
            _ring = null;
        }
        
        ////////////////////////////////////////////        
        // overrideable abstract methods
        ////////////////////////////////////////////

        override protected function transformChannel(data:Vector.<Number>, channel:Number, start:Number, numFrames:Number):void
        {
            // The delay line is just a Sample whose channels are used as a ring buffer.  Rather than
            // shifting values around in the channel vectors, the indices representing the "start" and "end" are moved
            // around.
            //
            if (_ring == null)
            {
                _bufferLength = Math.floor(_period * descriptor.rate);
                _ring = new Sample(descriptor, _bufferLength);
            }
            
            // Determine the index at which the new sampe will be placed in the delay ring;
            // this is also the index containing the oldest sample in the ring, so extract
            // that first and mix it into the output.
            // 
            var j:Number = start % _bufferLength;
            var ringData:Vector.<Number> = _ring.channelData[channel];
            for (var i:Number = 0; i < numFrames; i++)
            {
                var echo:Number = ringData[j];
                ringData[j] = data[i] + (echo * _decay);
                j++;
                if (j >= _bufferLength)
                {
                    j = 0;
                }
                data[i] += echo * _wet;
            }             
        }

        override public function clone():IAudioSource
        {
            return new EchoFilter(source.clone(), period, wet, decay);
        }
    }
}
