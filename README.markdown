Flite+hts_engine in Flash
--------------------------

A port of Flite + hts_engine running in Adobe Flash plugin

Paul Dixon (paul@edobashira.com)

Makes use of the following excellent opensource software:

  - [Flite + hts_engine ](http://hts-engine.sourceforge.net/)
  - [minimalcomps](http://www.minimalcomps.com)
  - [Standingwave](http://code.google.com/p/standingwave/)
  - [Swfobject](http://code.google.com/p/swfobject/)


## Installation and compilation under Cygwin/Windows ##

### Pre-requisites ###
  -  [Cygwin](http://cygwin.org/)
  - [Alchemy](http://labs.adobe.com/technologies/alchemy/) installed under Cygwin 
  -  [Flashdevelop](http://www.flashdevelop.org/wikidocs/index.php?title=Main_Page)  

### Build HTS Flash control###
From the cygwin terminal

cd /src/dic

alc-on

make

### Build GUI ###
Run Flashdevelop and build the project. 
obj/htsflashConfig.xml may need editing to reflect your system paths

### Disclaimer ###
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN 
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION 
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
