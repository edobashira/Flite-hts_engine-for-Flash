package 
{
		
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.KeyboardEvent;	
	import flash.net.FileReference;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.media.Microphone;		
	import flash.utils.ByteArray;
	import flash.events.SampleDataEvent;
	import flash.media.SoundChannel;
	import flash.media.Sound;	
	import flash.utils.Endian;
	import flash.utils.Dictionary;
	import flash.ui.Keyboard;
	
	import cmodule.hts.CLibInit;
	
	import com.bit101.components.Text;
	import com.bit101.components.PushButton;
	import com.bit101.components.InputText;
	import com.bit101.components.Panel;	
	import com.bit101.components.Label;

	import com.noteflight.standingwave2.elements.AudioDescriptor;
	import com.noteflight.standingwave2.elements.Sample; 
	import com.noteflight.standingwave2.formats.WaveFile;	
	import com.noteflight.standingwave2.output.AudioPlayer;
	import com.noteflight.standingwave2.elements.Sample;
	import com.noteflight.standingwave2.input.MicrophoneInput;
	
	/*flite_hts_engine \
     -td tree-dur.inf -tf tree-lf0.inf -tm tree-mgc.inf \
     -md dur.pdf         -mf lf0.pdf       -mm mgc.pdf \
     -df lf0.win1        -df lf0.win2      -df lf0.win3 \
     -dm mgc.win1        -dm mgc.win2      -dm mgc.win3 \
     -cf gv-lf0.pdf      -cm gv-mgc.pdf    -ef tree-gv-lf0.inf \
     -em tree-gv-mgc.inf -k  gv-switch.inf -ow output.wav \
     input.txt*/
	 	 
	public class Main extends Sprite 
	{		
		//-td tree-dur.inf -tf tree-lf0.inf -tm tree-mgc.inf \
		[Embed(source="../embed/tree-dur.inf ", mimeType="application/octet-stream")]
		private var _td:Class;
		[Embed(source="../embed/tree-lf0.inf", mimeType="application/octet-stream")]
		private var _tf:Class;	
		[Embed(source="../embed/tree-mgc.inf ", mimeType="application/octet-stream")]
		private var _tm:Class;
		
		//-md dur.pdf         -mf lf0.pdf       -mm mgc.pdf 
		[Embed(source="../embed/dur.pdf", mimeType="application/octet-stream")]
		private var _md:Class;
		[Embed(source="../embed/lf0.pdf", mimeType="application/octet-stream")]
		private var _mf:Class;
		[Embed(source="../embed/mgc.pdf", mimeType="application/octet-stream")]
		private var _mm:Class;
		
		//-df lf0.win1        -df lf0.win2      -df lf0.win3 \
		[Embed(source="../embed/lf0.win1", mimeType="application/octet-stream")]
		private var _df1:Class;
		[Embed(source="../embed/lf0.win2", mimeType="application/octet-stream")]
		private var _df2:Class;
		[Embed(source="../embed/lf0.win3", mimeType="application/octet-stream")]
		private var _df3:Class;
		
		//-dm mgc.win1        -dm mgc.win2      -dm mgc.win3 \
		[Embed(source="../embed/mgc.win1", mimeType="application/octet-stream")]
		private var _dm1:Class;
		[Embed(source="../embed/mgc.win2", mimeType="application/octet-stream")]
		private var _dm2:Class;
		[Embed(source="../embed/mgc.win3", mimeType="application/octet-stream")]
		private var _dm3:Class;
		
		//-cf gv-lf0.pdf      -cm gv-mgc.pdf    -ef tree-gv-lf0.inf \
		[Embed(source="../embed/gv-lf0.pdf", mimeType="application/octet-stream")]
		private var _cf:Class;
		[Embed(source="../embed/gv-mgc.pdf", mimeType="application/octet-stream")]
		private var _cm:Class;
		[Embed(source="../embed/tree-gv-lf0.inf", mimeType="application/octet-stream")]
		private var _ef:Class;
				
		//-em tree-gv-mgc.inf -k  gv-switch.inf -ow output.wav \
		[Embed(source="../embed/tree-gv-mgc.inf", mimeType="application/octet-stream")]
		private var _em:Class;
		[Embed(source="../embed/gv-switch.inf", mimeType="application/octet-stream")]
		private var _k:Class;
		[Embed(source="../embed/output.wav", mimeType="application/octet-stream")]
		private var _ow:Class;
		
		[Embed(source="../embed/input.txt", mimeType="application/octet-stream")]
		private var _input:Class;		
		
		private var _loader:CLibInit;
		private var _swc:Object;

		
		private var _byteArray:ByteArray;
		private var _sound:Sound;
		private var _soundChannel:SoundChannel;		
		
		private var _inputText:InputText;
		private var _pushbutton:PushButton;		
		private var _saveButton:PushButton;	
		private var _panel:Panel;	
		private var _aboutText:TextField;
		
		private var _player:AudioPlayer;
		
		private var pressedKeys:Dictionary;
		

		//private var mc:MovieClip;
		
		public function Main():void 
		{
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
					
		
		private function onSaveButtonClick(e:MouseEvent):void
        {
			_byteArray.clear();
			_swc.swcSynth(_byteArray, _inputText.text);	
			var file:FileReference = new FileReference;
			file.save(_byteArray, "hts_flash_test.wav");  							
		}
		
	
		

		private function keyListener( e:KeyboardEvent ):void
		{
			if( e.type == KeyboardEvent.KEY_DOWN )
			{
				if( pressedKeys[ e.keyCode ] == null )
				{
					pressedKeys[ e.keyCode ] = 1;
					keyPressed( e.keyCode );
				}
			}
			else
			{
				if( pressedKeys[ e.keyCode ] != null )
				{
					delete pressedKeys[ e.keyCode ];
					keyReleased( e.keyCode );
				}
			}
		}

		private function keyPressed( keyCode:uint ):void
		{
			if( keyCode == Keyboard.ENTER )
			{
			
			}
		}

		private  function keyReleased( keyCode:uint ):void
		{
			if( keyCode == Keyboard.ENTER )
			{
				_byteArray.clear();
				_swc.swcSynth(_byteArray, _inputText.text);			
				var sample:Sample = sample = new Sample(new AudioDescriptor(44100,1), 0);			
				var data1:Vector.<Number> = sample.channelData[0];
				_byteArray.position = 44; 
				var j:uint = 0;
				while (_byteArray.bytesAvailable > 0)
				{				
					var f:Number = _byteArray.readShort() / 32767.0;	
					data1[j++] = f;
					data1[j++] = f;
					data1[j++] = f;
				}
				_player.play(sample);	
			}
		}
		
		private function onPushButtonClick(e:MouseEvent):void
        {			
			_byteArray.clear();
			_swc.swcSynth(_byteArray, _inputText.text);			
			var sample:Sample = sample = new Sample(new AudioDescriptor(44100,1), 0);			
			var data1:Vector.<Number> = sample.channelData[0];
			_byteArray.position = 44; 
			var j:uint = 0;
			while (_byteArray.bytesAvailable > 0)
			{				
				var f:Number = _byteArray.readShort() / 32767.0;	
				data1[j++] = f;
				data1[j++] = f;
				data1[j++] = f;
			}
			_player.play(sample);			
        }
		
		private function writeData(e:SampleDataEvent):void
		{			
			while (_byteArray.bytesAvailable > 0)
			{
				var f:Number = _byteArray.readShort() / 32767.0;	
				e.data.writeFloat(f);
				e.data.writeFloat(f);
			}
		}
		
		
		
		private function init(e:Event = null):void 
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);
			
			_panel = new Panel(this, 0, 0);
			_panel.setSize(stage.stageWidth , stage.stageHeight);															
		
			_aboutText = new TextField();
			_aboutText.x = 20;
			_aboutText.y = 20;
			_aboutText.height = 20;
			_aboutText.width = 220;
			_aboutText.htmlText = "<FONT FACE=\"Arial,Helvetica,sans-serif\" SIZE=\"10\" COLOR=\"#909090\"> <a href=\"http://hts-engine.sourceforge.net\"> HTS Engine </a> Ported to Flash by <a href=\"http://www.lvcsr.com\"> Paul Dixon </a>  </FONT>";
			this.addChild(_aboutText);								
		
			_inputText = new InputText(_panel, 20, 80);			
			_inputText.text = "Enter your text here and then press the synthesize button or hit enter.";
			_inputText.width = 220;
			_inputText.height = 20;			
			_inputText.addEventListener( KeyboardEvent.KEY_DOWN, keyListener );
			_inputText.addEventListener( KeyboardEvent.KEY_UP, keyListener );

			
			_pushbutton =  new PushButton(_panel, 20, 50, "", onPushButtonClick);
			_pushbutton.label = "Synthesize";
			_pushbutton.width = 100;
			
			_saveButton =  new PushButton(_panel, 140, 50, "", onSaveButtonClick);
			_saveButton.label = "Synthesize To File";
			_saveButton.width = 100;
			
			_byteArray = new ByteArray();
			_byteArray.endian = Endian.LITTLE_ENDIAN;
			
			_sound = new Sound();
			
			_player = new AudioPlayer();			
			
			pressedKeys = new Dictionary();
			
			_loader = new CLibInit;			
			_swc = _loader.init();			
						
			 var td:ByteArray = new _td;
			 _loader.supplyFile("tree-dur.inf",td);
			 var tf:ByteArray = new _tf;
			 _loader.supplyFile("tree-lf0.inf",tf);
			 var tm:ByteArray = new _tm;
			 _loader.supplyFile("tree-mgc.inf", tm);			 
			 
			 var md:ByteArray = new _md;
			 _loader.supplyFile("dur.pdf", md);
			 var mf:ByteArray = new _mf;
			 _loader.supplyFile("lf0.pdf", mf);
			 var mm:ByteArray = new _mm;
			 _loader.supplyFile("mgc.pdf", mm);
			 
			 var df1:ByteArray = new _df1;
			 _loader.supplyFile("lf0.win1", df1);
			 var df2:ByteArray = new _df2;
			 _loader.supplyFile("lf0.win2", df2);
			 var df3:ByteArray = new _df3;
			 _loader.supplyFile("lf0.win3", df3);
			 
			 var dm1:ByteArray = new _dm1;
			 _loader.supplyFile("mgc.win1", dm1);
			 var dm2:ByteArray = new _dm2;
			 _loader.supplyFile("mgc.win2", dm2);
			 var dm3:ByteArray = new _dm3;
			 _loader.supplyFile("mgc.win3", dm3);
			 
			 var cf:ByteArray = new _cf;
			 _loader.supplyFile("gv-lf0.pdf", cf);
			 var cm:ByteArray = new _cm;
			 _loader.supplyFile("gv-mgc.pdf", cm);
			 var ef:ByteArray = new _ef;
			 _loader.supplyFile("tree-gv-lf0.inf", ef);			 
			 
			 var em:ByteArray = new _em;
			 _loader.supplyFile("tree-gv-mgc.inf", em);			 
			 var k:ByteArray = new _k;
			 _loader.supplyFile("gv-switch.inf", k);
			 
			_swc.swcInit(this);			
			
		
		}
		
	}
	
}