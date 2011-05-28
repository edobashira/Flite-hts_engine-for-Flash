package com.noteflight.standingwave2.output
{
    import com.noteflight.standingwave2.elements.AudioDescriptor;
    import com.noteflight.standingwave2.elements.IAudioSource;
    import com.noteflight.standingwave2.elements.Sample;
    
    import flash.events.Event;
    import flash.events.EventDispatcher;
    import flash.events.SampleDataEvent;
    import flash.media.SoundChannel;
    import flash.utils.getTimer;
 
    /** Dispatched when the currently playing sound has completed. */
    [Event(type="flash.events.Event",name="complete")]
    
    /**
     * A delegate object that takes care of the work for audio playback by moving data
     * from an IAudioSource into a SampleDataEvent's ByteArray.
     */
    public class AudioSampleHandler extends EventDispatcher
    {
        /** Reports % of CPU used after each SampleDataEvent based on last event interval */
        public var cpuPercentage:Number = 0;

        /** frames supplied for each SampleDataEvent */
        public var framesPerCallback:Number;
        
        /** Overall gain factor for output */
        public var gainFactor:Number = 0.5;
        
        /** The absolute frame number of the sample block at which the current source began playing */
        private var _startFrame:Number = 0;

        /** Timer value at conclusion of last sample block calculation, for CPU percentage determination */
        private var _lastSampleTime:Number = 0;
        
        /** Flag indicating that the current source has been examined during a sample data event. */
        private var _sourceStarted:Boolean;
        
        private var _totalLatency:Number = 0;
        private var _latencyCount:Number = 0;
        private var LATENCY_MAX_COUNT:Number = 10;

        // The SoundChannel that the output is playing through, really only needed for calculating latency
        private var _channel:SoundChannel;

        // If non-null, the audio source being currently rendered        
        private var _source:IAudioSource;
        
 
        public function AudioSampleHandler(framesPerCallback:Number = 4096)
        {
            this.framesPerCallback = framesPerCallback;
        }

        public function get source():IAudioSource
        {
            return _source;
        }

        public function set source(source:IAudioSource):void
        {
            _source = source;
        }

        public function set sourceStarted(sourceStarted:Boolean):void
        {
            _sourceStarted = sourceStarted;
        }
        
        public function set channel(channel:SoundChannel):void
        {
            _channel = channel;
        }


        /**
         * The latency in seconds.
         */
        public function get latency():Number
        {
            if (_latencyCount < LATENCY_MAX_COUNT)
            {
                return 0;
            }
            return _totalLatency / _latencyCount;
        }

        /**
         * The position in seconds relative to the start of the current source, else zero 
         */
        public function get position():Number
        {
            if (_channel != null && _source != null && _sourceStarted)
            {
                return (_channel.position / 1000.0) - (_startFrame / AudioDescriptor.RATE_44100);
            }
            return 0;
        }
        
        /**
         * Handle a request by the player for a block of samples. 
         */
        public function handleSampleData(e:SampleDataEvent):void
        {
            var now:Number = getTimer();
            
            // Determine latency based on skew between channel position and sample request position.
            if (_channel && position > 0 && _latencyCount < LATENCY_MAX_COUNT)
            {
                _totalLatency += (e.position / AudioDescriptor.RATE_44100) - (_channel.position / 1000.0);
                _latencyCount++; 
            }
            
            var endFrame:Number;
            var sample:Sample;
            var length:Number;
            
            var chan0:Vector.<Number>;
            var chan1:Vector.<Number>;
            var i:Number;
            
            // If the current source has never been seen here before, capture the starting
            // frame number at which its rendering is beginning.
            //
            if (!_sourceStarted)
            {
                _startFrame = e.position;
                _sourceStarted = true;
            }
            
            // Determine the frame at which we should start getting samples from the source.
            //
            var frame:Number = e.position - _startFrame;

            switch (_source.descriptor.rate)
            {
            case AudioDescriptor.RATE_44100:
                endFrame = Math.min(_source.frameCount, frame + framesPerCallback);
                length = Math.max(0, endFrame - frame);
                if (length == 0)
                {
                    break;
                }
                
                sample = _source.getSample(length);

                switch (sample.descriptor.channels)
                {
                case AudioDescriptor.CHANNELS_MONO:
                    chan0 = sample.channelData[0];
                    for (i = 0; i < length; i++)
                    {
                        e.data.writeFloat(chan0[i] * gainFactor);
                        e.data.writeFloat(chan0[i] * gainFactor);
                    }
                    break;
    
                case AudioDescriptor.CHANNELS_STEREO:
                    chan0 = sample.channelData[0];
                    chan1 = sample.channelData[1];
                    for (i = 0; i < length; i++)
                    {
                        e.data.writeFloat(chan0[i] * gainFactor);
                        e.data.writeFloat(chan1[i] * gainFactor);
                    }
                    break;
                }
                break; 

            case AudioDescriptor.RATE_22050:
                endFrame = Math.min(_source.frameCount * 2, frame + framesPerCallback);
                length = Math.max(0, endFrame - frame);
                if (length == 0)
                {
                    break;
                }
                
                var halfLength:Number = length >> 1;
                sample = _source.getSample(halfLength);
                var signal:Number;
                switch (sample.descriptor.channels)
                {
                case AudioDescriptor.CHANNELS_MONO:
                    chan0 = sample.channelData[0];
                    for (i = 0; i < halfLength ; i++)
                    {
                        signal = chan0[i] * gainFactor;
                        e.data.writeFloat(signal);
                        e.data.writeFloat(signal);
                        e.data.writeFloat(signal);
                        e.data.writeFloat(signal);
                    }
                    if ((length & 1) == 1)
                    {
                        signal = chan0[length-1] * gainFactor;
                        e.data.writeFloat(signal);
                        e.data.writeFloat(signal);
                    }
                    break;
    
                case AudioDescriptor.CHANNELS_STEREO:
                    chan0 = sample.channelData[0];
                    chan1 = sample.channelData[1];
                    for (i = 0; i < halfLength ; i++)
                    {
                        signal = chan0[i] * gainFactor;
                        e.data.writeFloat(chan0[i] * gainFactor);
                        e.data.writeFloat(chan1[i] * gainFactor);
                        e.data.writeFloat(chan0[i] * gainFactor);
                        e.data.writeFloat(chan1[i] * gainFactor);
                    }
                    if ((length & 1) == 1)
                    {
                        e.data.writeFloat(chan0[length-1] * gainFactor);
                        e.data.writeFloat(chan1[length-1] * gainFactor);
                    }
                    break;
                }
                break; 

            }

            if (length == 0)
            {
                _source = null;
                _sourceStarted = false;
                dispatchEvent(new Event(Event.COMPLETE));
            }
            else if (length > 0 && length < framesPerCallback)
            {
                // Fill out remainder of sample block source could not supply all frames.  
                for (i = length; i < framesPerCallback; i++)
                {
                    e.data.writeFloat(0);
                    e.data.writeFloat(0);
                }
            }

            // Calculate CPU utilization
            calculateCpu(now);
        }

        private function calculateCpu(now:Number):void
        {
            if (_lastSampleTime > 0)
            {
                cpuPercentage = 100 * (getTimer() - now) / (now - _lastSampleTime);
                //trace("cpu:", cpuPercentage, "latency:", latency, "interval:", now - _lastSampleTime);
            }
            _lastSampleTime = now;
        }
    }
}
