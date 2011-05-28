package com.joeberkovitz.simpleworld.editor
{
    import com.joeberkovitz.simpleworld.model.Composition;
    import com.joeberkovitz.simpleworld.model.SonicElement;
    import com.joeberkovitz.simpleworld.model.SoundClip;
    import com.joeberkovitz.simpleworld.model.Tone;
    import com.noteflight.standingwave2.elements.AudioDescriptor;
    import com.noteflight.standingwave2.elements.IAudioSource;
    import com.noteflight.standingwave2.filters.EnvelopeFilter;
    import com.noteflight.standingwave2.filters.ResamplingFilter;
    import com.noteflight.standingwave2.performance.IPerformance;
    import com.noteflight.standingwave2.performance.ListPerformance;
    import com.noteflight.standingwave2.sources.SineSource;
    import com.noteflight.standingwave2.sources.SoundSource;
    import com.noteflight.standingwave2.utils.AudioUtils;
    
    public class SoundRenderer
    {
        public static const PIX_PER_SECOND:Number = 500;
        public static const PIX_PER_OCTAVE:Number = 100;
        public static const PIX_PER_DB:Number = 2;
        public static const CENTER_Y:Number = 400;
        
        public function renderComposition(c:Composition):IPerformance
        {
            var p:ListPerformance = new ListPerformance();
            for each (var element:SonicElement in c.elements)
            {
                if (element is Tone)
                {
                    addToneSource(p, element as Tone)
                }
                else if (element is SoundClip)
                {
                    addSoundClipSource(p, element as SoundClip);
                }
            }

            return p;            
        }
        
        public static function heightToAmplitude(h:Number):Number
        {
            var amplitude:Number = AudioUtils.decibelsToFactor(-20 + (h / PIX_PER_DB));
            amplitude = Math.min(amplitude, 1);
            return amplitude;
        }
        
        public static function yToFactor(y:Number):Number
        {
            return Math.exp((CENTER_Y - y) * Math.LN2 / PIX_PER_OCTAVE);
        }
        
        private function addToneSource(p:ListPerformance, tone:Tone):void
        {
            var duration:Number = tone.width / PIX_PER_SECOND;
            var startTime:Number = tone.x / PIX_PER_SECOND;
            var frequency:Number = 440 * yToFactor(tone.y);
            var amplitude:Number = heightToAmplitude(tone.height);
            var source:IAudioSource = new SineSource(new AudioDescriptor(), duration + 0.1, frequency, amplitude);
            source = new EnvelopeFilter(source, 0.01, 0, 1, duration - 0.01, 0.1);
            p.addSourceAt(startTime, source);
        }
        
        private function addSoundClipSource(p:ListPerformance, clip:SoundClip):void
        {
            var source:IAudioSource = new SoundSource(clip.sound);
            var startTime:Number = clip.x / PIX_PER_SECOND;
            var amplitude:Number = heightToAmplitude(clip.height);
            source = new ResamplingFilter(source, yToFactor(clip.y));
            var duration:Number = source.frameCount / source.descriptor.rate;
            source = new EnvelopeFilter(source, 0, 0, amplitude, duration, 0);
            p.addSourceAt(startTime, source);
        }
    }
}