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
    import com.noteflight.standingwave2.elements.Sample;
    
    /**
     * AbstractSource is an implementation superclass for IAudioSource implementations
     * that wish to generate their output on a per-channel basis.  An AbstractSource
     * has standard amplitude and duration properties.
     */
    public class AbstractSource implements IAudioSource
    {
        /** Audio descriptor for this source. */
        protected var _descriptor:AudioDescriptor;
        protected var _position:Number;

        public var amplitude:Number;
        public var duration:Number;
        
        public static const MAX_DURATION:Number = int.MAX_VALUE;

        /**
         * Create an AbstractSource of a particular duration and amplitude. 
         * @param descriptor an AudioDescriptor for the source's audio
         * @param duration the duration of the source's output
         * @param amplitude the amplitude of the source's output
         */
        public function AbstractSource(descriptor:AudioDescriptor, duration:Number = MAX_DURATION, amplitude:Number = 1.0)
        {
            _descriptor = descriptor;
            this.amplitude = amplitude;
            this.duration = duration;
            _position = 0;
        }
        
        protected function generateChannel(data:Vector.<Number>, channel:Number, numFrames:Number):void
        {
            throw new Error("generateChannel() not overridden");
        }

        ////////////////////////////////////////////        
        // IAudioSource interface implementation
        ////////////////////////////////////////////        
        
        /**
         * Get the AudioDescriptor for this Sample.
         */
        public function get descriptor():AudioDescriptor
        {
            return _descriptor;
        }
        
        public function get frameCount():Number
        {
            return Math.floor(duration * _descriptor.rate);
        }

        public function get position():Number
        {
            return _position;
        }
        
        public function resetPosition():void
        {
            _position = 0;
        }
        
        public function getSample(numFrames:Number):Sample
        {
            var sample:Sample = new Sample(descriptor, numFrames);
            for (var c:Number = 0; c < descriptor.channels; c++)
            {
                var data:Vector.<Number> = new Vector.<Number>(numFrames, true); 
                sample.channelData[c] = data;
                generateChannel(data, c, numFrames);
            }
            _position += numFrames;
            
            return sample;
        }
        
        public function clone():IAudioSource
        {
            throw new Error("clone() not overridden");
        }
    }
}
