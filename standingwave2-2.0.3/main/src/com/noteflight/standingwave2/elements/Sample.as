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


package com.noteflight.standingwave2.elements
{
    import __AS3__.vec.Vector;
    
    /**
     * Sample is the fundamental audio source in StandingWave, and is simply a set of buffers
     * containing the individual numeric samples of an audio signal.  The <code>channelData</code>
     * property is an Array of channel buffers, whose length is the number of channels
     * specified by the descriptor.  Each channel buffer is a Vector of Numbers, whose elements
     * are the individual samples for that channel, ranging between -1 and +1.
     */
    public class Sample implements IAudioSource, IRandomAccessSource
    {
        /** Array of Vectors of data samples as Numbers, one Vector per channel. */
        public var channelData:Array;
        
        /** Audio descriptor for this sample. */
        private var _descriptor:AudioDescriptor;
        
        /** Audio cursor position, expressed as a sample frame index. */
        private var _position:Number;

        /**
         * Construct a new, empty Sample with some specified audio format. 
         * @param descriptor an AudioDescriptor specifying the audio format of this sample.
         * @param frames the number of frames in this Sample.  If omitted or negative, no channel
         * data vectors are created.  If zero, then zero-length vectors are created and may be
         * grown in size.  If positive, then fixed-size vectors are created, and will contain zeroes.
         */
        public function Sample(descriptor:AudioDescriptor, frames:Number = -1)
        {
            _descriptor = descriptor;
            channelData = [];
            if (frames >= 0)
            {
                for (var c:Number = 0; c < channels; c++)
                {
                    channelData[c] = new Vector.<Number>(frames, frames > 0);
                }
            }
            _position = 0;
        }
        
        /**
         * The number of channels in this sample
         */
        public function get channels():Number
        {
            return _descriptor.channels;
        }
        
        /**
         * Return a sample for the given channel and frame index
         * @param channel the channel index of the sample
         * @param index the frame index of the sample within that channel
         */
        public function getChannelSample(channel:Number, index:Number):Number
        {
            return Vector.<Number>(channelData[channel])[index];
        }
        
        /**
         * Return an interpolated sample for a non-integral sample position.
         * Interpolation is always done within the same channel.
         */
        public function getInterpolatedSample(channel:Number, pos:Number):Number
        {
            var intPos:Number = int(pos);
            var fracPos:Number = pos - intPos;
            var data:Vector.<Number> = channelData[channel];
            var s1:Number = data[intPos];
            var s2:Number = data[intPos+1];
            return s1 + (fracPos * (s2 - s1));
        }
        
        /**
         * Return duration in seconds
         */
        public function get duration():Number
        {
            return Number(frameCount) / _descriptor.rate;
        }
        
        /**
         * Clear this sample. 
         */
        public function clear():void
        {
            var n:Number = frameCount;
            for (var c:Number = 0; c < channels; c++)
            {
                var data:Vector.<Number> = channelData[c];
                for (var i:Number = 0; i < n; i++)
                {
                    data[i] = 0;
                }
            }
        }
        
        ////////////////////////////////////////////        
        // IRandomAccessSource/IAudioSource interface implementation
        ////////////////////////////////////////////        
        
        /**
         * @inheritDoc
         */
        public function get descriptor():AudioDescriptor
        {
            return _descriptor;
        }
        
        /**
         * @inheritDoc
         */
        public function get frameCount():Number
        {
            return channelData[0].length;
        }
        
        public function set frameCount(value:Number):void
        {
            for (var c:Number = 0; c < channels; c++)
            {
                var data:Vector.<Number> = channelData[c];
                data.length = value;
            }
        }

        /**
         * @inheritDoc
         */
        public function get position():Number
        {
            return _position;
        }
        
        public function set position(p:Number):void
        {
            _position = p;
        }
        
        /**
         * @inheritDoc
         */
        public function resetPosition():void
        {
            _position = 0;
        }
        
        /**
         * @inheritDoc
         */
        public function getSampleRange(fromOffset:Number, toOffset:Number):Sample
        {
            var sample:Sample = new Sample(descriptor);
            for (var c:Number = 0; c < channels; c++)
            {
                sample.channelData[c] = Vector.<Number>(channelData[c]).slice(fromOffset, toOffset);
            }
            return sample;
        }

        /**
         * @inheritDoc
         */
        public function getSample(numFrames:Number):Sample
        {
            var sample:Sample = getSampleRange(_position, _position + numFrames);
            _position += numFrames;
            return sample;
        }
        
        /**
         * Clone this Sample.  Note that the channel data is shared between the
         * original and the clone, since channel data inside a Sample should never
         * be mutated except for temporary Samples used inside a filter pipeline.
         */
        public function clone():IAudioSource
        {
            var sample:Sample = new Sample(descriptor);
            for (var c:Number = 0; c < channels; c++)
            {
                sample.channelData[c] = channelData[c];
            }
            return sample;
        }
    }
}
