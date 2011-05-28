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


package com.noteflight.standingwave2.sources
{
    import __AS3__.vec.Vector;
    
    import com.noteflight.standingwave2.elements.AudioDescriptor;
    import com.noteflight.standingwave2.elements.IAudioSource;
    
    /**
     * A SineSource provides a source whose signal in all channels is a pure sine wave of a given frequency. 
     */
    public class SineSource extends AbstractSource
    {
        private var _frequency:Number = 0;
        public var phase:Number = 0;

        public function SineSource(descriptor:AudioDescriptor, duration:Number, frequency:Number, amplitude:Number = 0.5)
        {
            super(descriptor, duration, amplitude);
            this.frequency = frequency;
        }

        /**
         * The frequency of this sine wave. 
         */
        public function get frequency():Number
        {
            return _frequency;
        }
        
        public function set frequency(value:Number):void
        {
            // When the frequency changes, preserve the phase angle at the current position
            // to avoid discontinuities in the output.  This requires calculating the phase angle
            // before and after the change, and then adjusting the phase to cancel the difference.
            //
            var oldPhase:Number = _position * _frequency / descriptor.rate;
            oldPhase = (oldPhase - Math.floor(oldPhase)) * 2 * Math.PI; 
            var newPhase:Number = _position * value / descriptor.rate;
            newPhase = (newPhase - Math.floor(newPhase)) * 2 * Math.PI; 

            _frequency = value;
            phase += oldPhase - newPhase;
        }
        
        override public function resetPosition():void
        {
            super.resetPosition();
            phase = 0;
        }
        
        override protected function generateChannel(data:Vector.<Number>, channel:Number, numFrames:Number):void
        {
            var factor:Number = frequency * Math.PI * 2 / descriptor.rate;
            for (var i:Number = 0; i < numFrames; i++)
            {
                data[i] = Math.sin((_position + i) * factor + phase) * amplitude;
            }
        }
        
        override public function clone():IAudioSource
        {
            return new SineSource(descriptor, duration, frequency, amplitude);
        }
    }
}
