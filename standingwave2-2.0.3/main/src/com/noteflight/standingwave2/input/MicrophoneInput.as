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


package com.noteflight.standingwave2.input
{
    import com.noteflight.standingwave2.elements.AudioDescriptor;
    import com.noteflight.standingwave2.elements.Sample;
    
    import flash.events.Event;
    import flash.events.EventDispatcher;
    import flash.events.SampleDataEvent;
    import flash.media.Microphone;
    import flash.utils.ByteArray;
    import flash.utils.Endian;
    
    /** Dispatched as data is recorded to reflect a change in the underlying sample. */
    [Event(type="flash.events.Event",name="change")]
    
    /**
     * The MicrophoneInput class records audio input to a Sample object in which digitized audio
     * accumulates.
     */    
    public class MicrophoneInput extends EventDispatcher
    {
        // Sample to which acquired audio is appended
        private var _sample:Sample;
        
        // microphone from which audio is acquired
        private var _microphone:Microphone;
        
        // flag indicating that recording is active
        private var _recording:Boolean = false;
        
        public function MicrophoneInput(microphone:Microphone)
        {
            _microphone = microphone;
            clear();
        }
        
        public function clear():void
        {
            var rate:Number;
            switch (_microphone.rate)
            {
                case 5:
                    rate = AudioDescriptor.RATE_5512;
                    break;

                case 8:
                    rate = AudioDescriptor.RATE_8000;
                    break;

                case 11:
                    rate = AudioDescriptor.RATE_11025;
                    break;

                case 22:
                    rate = AudioDescriptor.RATE_22050;
                    break;

                case 44:
                default:
                    rate = AudioDescriptor.RATE_44100;
                    break;
            }
            var desc:AudioDescriptor = new AudioDescriptor(rate, 1);
            _sample = new Sample(desc, 0);
        }
        
        public function record():void
        {
            if (_recording)
            {
                return;
            }
            
            _recording = true;
            _microphone.addEventListener(SampleDataEvent.SAMPLE_DATA, handleSampleData);
        } 
        
        public function stop():void
        {
            if (!_recording)
            {
                return;
            }
            
            _recording = false;
            _microphone.removeEventListener(SampleDataEvent.SAMPLE_DATA, handleSampleData);
        }
        
        public function get sample():Sample
        {
            return _sample;
        }
        
        [Bindable("change")]
        public function get frameCount():Number
        {
            return _sample.frameCount;
        } 
        
        public function get recording():Boolean
        {
            return _recording;
        }
        
        private function handleSampleData(e:SampleDataEvent):void
        {
            var data:ByteArray = e.data;
            data.endian = Endian.LITTLE_ENDIAN;   // HACK to work around Adobe bug
            var channel:Vector.<Number> = _sample.channelData[0];
            
            var j:Number = _sample.frameCount;
            var count:Number = j + data.bytesAvailable / 4;
            _sample.frameCount = count; 
            while (j < count)
            {
                channel[j++] = data.readFloat();
            }
            
            dispatchEvent(new Event(Event.CHANGE));
        }
    }
}
