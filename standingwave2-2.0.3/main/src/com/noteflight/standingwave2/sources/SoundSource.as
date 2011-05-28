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
    import com.noteflight.standingwave2.elements.IRandomAccessSource;
    import com.noteflight.standingwave2.elements.Sample;
    
    import flash.media.Sound;
    import flash.utils.ByteArray;
    
    /**
     * A SoundSource serves as a source of stereo 44.1k sound extracted from an underlying
     * Flash Player Sound object. 
     */
    public class SoundSource extends AbstractSource implements IRandomAccessSource
    {
        /** Underlying sound for this source. */
        private var _sound:Sound;

        public function SoundSource(sound:Sound)
        {
            super(new AudioDescriptor(44100, 2), (sound.length / 1000.0), 1.0);
            _sound = sound;
        }

        public function get sound():Sound
        {
            return _sound;
        }
        
        override public function getSample(numFrames:Number):Sample
        {
            var sample:Sample = new Sample(descriptor, numFrames);
            var chan0:Vector.<Number> = sample.channelData[0];
            var chan1:Vector.<Number> = sample.channelData[1];
            
            var bytes:ByteArray = new ByteArray();
            var numSamples:Number = _sound.extract(bytes, numFrames, position);
            bytes.position = 0;

            for (var i:Number = 0; i < numSamples; i++)
            {
                chan0[i] = bytes.readFloat();
                chan1[i] = bytes.readFloat();
            }
            
            // Fix Issue 1: a sound's reported length can slightly exceed the actual number
            // of samples available from it, so fill out the returned signal with zeroes.
            //
            while (i < numFrames)
            {
                chan0[i] = 0;
                chan1[i] = 0;
                i++;
            }
            
            _position += numFrames;
            
            return sample;
        }
        
        public function getSampleRange(fromOffset:Number, toOffset:Number):Sample {
            _position = fromOffset;
            return getSample(toOffset - fromOffset);
        }
        
        override public function clone():IAudioSource
        {
            return new SoundSource(sound);
        }
    }
}
