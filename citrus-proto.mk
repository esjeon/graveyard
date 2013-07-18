#
# This is a Makefile, and a prototype of citrus, a static website generator.
#

OUTDIR  := ./out
RESDIR  := ./res

.SUFFIXES:
.SUFFIXES: .md .htm .html .json .ejs .js

# Input files
MDS     := $(shell find * -path $(OUTDIR) -or -path $(RESDIR) -prune -or -name "*.md")
EJSS    := $(shell find * -path $(OUTDIR) -or -path $(RESDIR) -prune -or -name "*.ejs")
EJSPS   := $(shell find * -path $(OUTDIR) -or -path $(RESDIR) -prune -or -name "*.ejsp")

# Intermediate files (names only)
METAS   := $(MDS:%.md=%.json)
HTMLS   := $(MDS:%.md=%.htm)
TPLS    := $(EJSS:%.ejs=%.js)

# Tags
TAGDATA := $(RESDIR)/_tags.json

# prepend output paths
METAS   := $(addprefix $(RESDIR)/,$(METAS))
HTMLS   := $(addprefix $(RESDIR)/,$(HTMLS))
TPLS    := $(addprefix $(RESDIR)/tpl/,$(TPLS))

OUTPUT := $(HTMLS:$(RESDIR)/%.htm=$(OUTDIR)/%.html)
OUTPUT += $(EJSPS:$(RESDIR)/%.ejsp=$(OUTDIR)/%.html)
.SECONDARY: $(METAS) $(HTMLS) $(TPLS)

.PHONY: all clean targets

targets:
	@echo "METAS ="
	@echo "$(METAS)" | sed "s/ /\n\t/g; s/^/\t/;"
	@echo "HTMLS ="
	@echo "$(HTMLS)" | sed "s/ /\n\t/g; s/^/\t/;"
	@echo "TPLS  ="
	@echo "$(TPLS)" | sed "s/ /\n\t/g; s/^/\t/;"
	@echo "OUTPUT="
	@echo "$(OUTPUT)" | sed "s/ /\n\t/g; s/^/\t/;"

all: $(OUTPUT)

clean:
	@rm -rfv $(RESDIR)
	@rm -rfv $(OUTDIR)/*

# Compile Markdown
.ONESHELL:
$(RESDIR)/%.htm $(RESDIR)/%.json: %.md
	@
	echo "  MD   $<"
	mkdir -p $(dir $@)
	cat << EOF | node
	  var fs = require('fs'), marked = require('marked')
	    , code = fs.readFileSync('$<', 'utf8')
	    , meta = { path: '$*'
	             }
	    , errf = function(e){ if(e) throw e }
	  code = code.replace(/^@([^:]+):(.*)$$/mg, function(line, name, val) {
	    if ((name = name.trim().toLowerCase()) == 'tags') name = 'tag'
	    meta[name] = val.trim()
	    return ''
	  })
	  if (meta.tag) meta.tag = meta.tag.split(',').map(function(s){ return s.trim() })
	  if (!meta.template) meta.template = "default"
	  if (!meta.title) meta.tile = "$*"
	  fs.writeFile('$(RESDIR)/$*.htm', marked(code), errf)
	  fs.writeFile('$(RESDIR)/$*.json', JSON.stringify(meta), errf)
	EOF

# Compile EJS template
.ONESHELL:
$(RESDIR)/tpl/%.js: %.ejs
	@
	echo "  EJS  $<"
	mkdir -p $(dir $@)
	cat << EOF | node
	  var fs = require('fs'), ejs = require('ejs')
	    , code = fs.readFileSync('$<', 'utf8')
	    , opts = { filename: "$<"
	             , client: true
	             , compileDebug: true
	             }
	    , tplf = ejs.compile(code, opts)
	  fs.writeFileSync('$@', "module.exports = " + tplf.toString())
	EOF

# Combine tag data
.ONESHELL:
$(TAGDATA): $(METAS)
	@
	echo "  TAGS ....."
	cat << EOF | node
	  var fs = require('fs')
	    , tags = {}
	  "$(METAS)".trim().split(/\s+/).forEach(function(path) {
	    var meta = JSON.parse(fs.readFileSync(path.trim(), 'utf8'))
	      if (meta.tag) meta.tag.forEach(function(tag) {
	      if (! tags[tag]) tags[tag] = []
	      tags[tag].push({ path: meta.path
	                     , title: meta.title
	                     })
	    })
	  })
	  fs.writeFileSync('$@', JSON.stringify(tags))
	EOF

# Output from Markdown
.ONESHELL:
$(OUTDIR)/%.html: $(RESDIR)/%.htm $(RESDIR)/%.json $(TPLS) $(TAGDATA)
	@
	echo "  HTML $*.htm"
	mkdir -p $(dir $@)
	cat << EOF | node
	  var fs = require('fs')
	    , meta = JSON.parse(fs.readFileSync('$(RESDIR)/$*.json', 'utf8'))
	    , tplf = (meta.template && fs.existsSync('$(RESDIR)/tpl/'+meta.template+'.js'))
	      ? require('./$(RESDIR)/tpl/'+meta.template+'.js')
	      : function(data){ return data.body }
	  meta.body = fs.readFileSync('$<', 'utf8')
	  fs.writeFileSync('$@', tplf(meta))  
	EOF

# Output from EJS Pages
.ONESHELL:
$(OUTDIR)/%.html: %.ejsp $(METAS) $(TAGDATA)
	@
	echo "  EJSP $*.ejsp"
	mkdir -p $(dir $@)
	cat << EOF | node
	  var fs = require('fs'), ejs = require('ejs')
	    , code = fs.readFileSync('$<', 'utf8')
	    , pagef = ejs.compile(code, {client: true})
	EOF
