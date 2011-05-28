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
    import com.noteflight.standingwave2.elements.IAudioSource;
    
    /**
     * GainFilter applies a fixed gain factor to the underlying source.
     */
    public class GainFilter extends AbstractFilter
    {
        /** The gain factor applied. */
        public var gain:Number;
        
        /**
         * Create a new GainFilter. 
         * @param source the underlying audio source
         * @param gain the gain factor
         */
        public function GainFilter(source:IAudioSource, gain:Number)
        {
            super(source);
            this.gain = gain;
        }
                
        override protected function transformChannel(data:Vector.<Number>, channel:Number, start:Number, numFrames:Number):void
        {
            for (var i:Number = 0; i < numFrames; i++)
            {
                data[i] *= gain;
            }
        }

        override public function clone():IAudioSource
        {
            return new GainFilter(source.clone(), gain);
        }
    }
}
