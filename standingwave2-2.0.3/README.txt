StandingWave README
May 14, 2009

ABOUT THIS PROJECT

StandingWave is an AS3 code library designed for high level control of
the Flash Player's SampleDataEvent API for streaming audio output.  It
is based on a subset of the audio engine used by the Noteflight Score
Editor (see http://www.noteflight.com), and technically is titled
StandingWave2; StandingWave1 was never made available as open source.

The goal of StandingWave is to encapsulate the following kinds of
objects, permitting them to be easily chained together and combined to
produce complex, dynamic audio output:

- audio sources (MP3 or WAV files, algorithmic sound generators...)
- audio filters (echo, envelope shaping, equalization...)
- timed sequences of audio sources, which may be hierarchically composed

There are no fundamental musical concepts embodied in StandingWave,
but it may be straightforwardly extended with such, for instance by
reading MIDI files or by writing utility classes to manage tones,
scales, instruments, and so forth.


CONTENTS

main/
        The StandingWave code library and SWC.

test/
        A FlexUnit test suite for StandingWave.

example/
        A simple example demonstrating StandingWave features.

soundworld/
        A simple visual editor for an imaginary "notation" based on the Google Code
        Moccasin library.  Look at the com.joeberkovitz.simpleworld.editor.SoundRenderer
        class to go straight to the code where the sound is generated.


FLASH and FLEX VERSION REQUIREMENTS

StandingWave requires Flash Player 10.  It cannot be built or used
with Flash Player 9 or earlier.

The library code is intended to be compiled with the Flex MXMLC
compiler, not with the Flash IDE.  However there are no Flex APIs
actually used in StandingWave.  The [Bindable] metadata tag is used in
one file, but may be easily removed for any Flash-only projects with
no harm done.


DOCUMENTATION

Take a look at doc/index.html!

Also, the code is extensively commented, there is a unit test suite,
and the example programs exercise most aspects of the library.


CONTRIBUTIONS WELCOME

Please contact Joe Berkovitz at joe@noteflight.com if you are
interested in contributing to this project.


LICENSE

All materials in this project Copyright (c) 2009 Noteflight LLC.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
