SRCS= htssrc/HTS_audio.c    htssrc/cst_phoneset.c      htssrc/flite_hts_flash_engine.c htssrc/HTS_engine.c    htssrc/cst_regex.c         htssrc/regexp.c htssrc/HTS_gstream.c                 htssrc/cmu_lts_model.c              htssrc/cst_relation.c      htssrc/us_aswd.c htssrc/HTS_label.c                   htssrc/cmu_lts_rules.c              htssrc/cst_string.c        htssrc/us_expand.c htssrc/HTS_misc.c                    htssrc/cmu_us_kal.c                 htssrc/cst_synth.c         htssrc/us_ffeatures.c htssrc/HTS_model.c                   htssrc/cst_alloc.c                  htssrc/cst_tokenstream.c   htssrc/us_gpos.c htssrc/HTS_pstream.c                 htssrc/cst_cart.c                   htssrc/cst_utt_utils.c     htssrc/us_int_accent_cart.c htssrc/HTS_sstream.c                 htssrc/cst_error.c                  htssrc/cst_utterance.c     htssrc/us_int_tone_cart.c htssrc/HTS_vocoder.c                 htssrc/cst_features.c               htssrc/cst_val.c           htssrc/us_nums_cart.c htssrc/cmu_lex.c                     htssrc/cst_ffeature.c               htssrc/cst_val_const.c     htssrc/us_phoneset.c htssrc/cmu_lex_data.c                htssrc/cst_file_stdio.c             htssrc/cst_val_user.c      htssrc/us_phrasing_cart.c     htssrc/cst_item.c                   htssrc/cst_voice.c         htssrc/us_postlex.c htssrc/cmu_lex_entries.c             htssrc/cst_lexicon.c                htssrc/flite.c             htssrc/us_text.c htssrc/cst_lts.c                    htssrc/flite_hts_engine.c  htssrc/usenglish.c
SRCTYPE=c
CC=gcc

OUTPUT= hts
CFLAGS= $(WARN)  -DFLASH -DNO_ASM -DFLITE_PLUS_HTS_ENGINE -Isrc
ifeq ($(DEBUG),1)
OPTIM= -g
OBJDIR= Debug
else
OPTIM= -O2
OBJDIR= Release
endif
OUTPUTFILE= $(OUTPUT).swc
OUTPUTDIR= lib

all: $(OBJDIR)/$(OUTPUTFILE)
	@mkdir -p $(OUTPUTDIR)
	cp $(OBJDIR)/$(OUTPUTFILE) $(OUTPUTDIR)/$(OUTPUTFILE)


VPATH += $(dir $(SRCS))
OBJS= $(patsubst %.$(SRCTYPE),$(OBJDIR)/%.o,$(notdir $(SRCS)))
$(OBJDIR)/$(OUTPUTFILE): $(OBJS)
	@#export ACHACKS_TMPS=1; keeps the generated .as file
	export ACHACKS_TMPS=0; cd $(OBJDIR); $(CC) $(notdir $(OBJS)) $(OPTIM) -swc -o $(OUTPUTFILE)
	
INCLUDES=
$(OBJDIR)/%.o: %.$(SRCTYPE)
	@mkdir -p $(OBJDIR)
	@rm -f $@
	cd $(OBJDIR); $(CC) $(CFLAGS) $(INCLUDES) $(OPTIM) -c ../$< -o $(notdir $@)
	
clean:
	@rm -f $(OBJS)
	



