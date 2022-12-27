/*
 * Generated by ./misc/optlib2c from optlib/gperf.ctags, Don't edit this manually.
 */
#include "general.h"
#include "parse.h"
#include "routines.h"
#include "field.h"
#include "xtag.h"


static void initializeGPerfParser (const langType language)
{

	addLanguageRegexTable (language, "main");
	addLanguageRegexTable (language, "comment");
	addLanguageRegexTable (language, "codeinc");
	addLanguageRegexTable (language, "keywordsec");
	addLanguageRegexTable (language, "functions");
	addLanguageRegexTable (language, "structdec");
	addLanguageRegexTable (language, "struct");
	addLanguageRegexTable (language, "structbody");

	addLanguageTagMultiTableRegex (language, "main",
	                               "^/\\*",
	                               "", "", "{tenter=comment}", NULL);
	addLanguageTagMultiTableRegex (language, "main",
	                               "^%\\{",
	                               "", "", "{tjump=codeinc}{_guest=C,0end,}", NULL);
	addLanguageTagMultiTableRegex (language, "main",
	                               "^struct[ \t]",
	                               "", "", "{_guest=C,0start,}{tenter=struct}", NULL);
	addLanguageTagMultiTableRegex (language, "main",
	                               "^%%",
	                               "", "", "{tjump=keywordsec}", NULL);
	addLanguageTagMultiTableRegex (language, "main",
	                               "^%define[ \t]+(hash-function-name|lookup-function-name|class-name|string-pool-name)[ \t]+([_a-zA-Z:][_a-zA-Z:0-9]*)[^\n]*\n",
	                               "", "", ""
		"{{\n"
		"  \\2\n"
		"  << (hash-function-name)   /hfunc\n"
		"     (lookup-function-name) /lfunc\n"
		"     (class-name)           /class\n"
		"     (string-pool-name)     /strpool >> \\1 get\n"
		"  @2\n"
		"  _tag _commit pop\n"
		"}}", NULL);
	addLanguageTagMultiTableRegex (language, "main",
	                               "^[^\n]*\n",
	                               "", "", "", NULL);
	addLanguageTagMultiTableRegex (language, "comment",
	                               "^[^*]+",
	                               "", "", "", NULL);
	addLanguageTagMultiTableRegex (language, "comment",
	                               "^\\*/",
	                               "", "", "{tleave}", NULL);
	addLanguageTagMultiTableRegex (language, "comment",
	                               "^.",
	                               "", "", "", NULL);
	addLanguageTagMultiTableRegex (language, "codeinc",
	                               "^[^%]+",
	                               "", "", "", NULL);
	addLanguageTagMultiTableRegex (language, "codeinc",
	                               "^%\\}",
	                               "", "", "{tjump=structdec}{_guest=,,0start}", NULL);
	addLanguageTagMultiTableRegex (language, "codeinc",
	                               "^.",
	                               "", "", "", NULL);
	addLanguageTagMultiTableRegex (language, "keywordsec",
	                               "^%%",
	                               "", "", "{tjump=functions}{_guest=C,0end,}", NULL);
	addLanguageTagMultiTableRegex (language, "keywordsec",
	                               "^([^\n,]+)[^\n]*\n?",
	                               "\\1", "k", "", NULL);
	addLanguageTagMultiTableRegex (language, "keywordsec",
	                               "^[^\n]*\n",
	                               "", "", "", NULL);
	addLanguageTagMultiTableRegex (language, "functions",
	                               "^.+",
	                               "", "", "{_guest=,,0end}", NULL);
	addLanguageTagMultiTableRegex (language, "structdec",
	                               "^struct[ \t]",
	                               "", "", "{_guest=C,0start,}{tenter=struct}", NULL);
	addLanguageTagMultiTableRegex (language, "structdec",
	                               "^%%",
	                               "", "", "{tjump=keywordsec}", NULL);
	addLanguageTagMultiTableRegex (language, "structdec",
	                               "^%define[ \t]+(hash-function-name|lookup-function-name|class-name|string-pool-name)[ \t]+([_a-zA-Z:][_a-zA-Z:0-9]*)[^\n]*\n",
	                               "", "", ""
		"{{\n"
		"  \\2\n"
		"  << (hash-function-name)   /hfunc\n"
		"     (lookup-function-name) /lfunc\n"
		"     (class-name)           /class\n"
		"     (string-pool-name)     /strpool >> \\1 get\n"
		"  @2\n"
		"  _tag _commit pop\n"
		"}}", NULL);
	addLanguageTagMultiTableRegex (language, "structdec",
	                               "^[^\n]*\n",
	                               "", "", "", NULL);
	addLanguageTagMultiTableRegex (language, "struct",
	                               "^\\{",
	                               "", "", "{tenter=structbody}", NULL);
	addLanguageTagMultiTableRegex (language, "struct",
	                               "^/\\*",
	                               "", "", "{tenter=comment}", NULL);
	addLanguageTagMultiTableRegex (language, "struct",
	                               "^;",
	                               "", "", "{tleave}{_guest=,,0end}", NULL);
	addLanguageTagMultiTableRegex (language, "struct",
	                               "^%",
	                               "", "", "{tleave}{_guest=,,0end}{_advanceTo=0start}", NULL);
	addLanguageTagMultiTableRegex (language, "struct",
	                               "^.",
	                               "", "", "", NULL);
	addLanguageTagMultiTableRegex (language, "structbody",
	                               "^\\}",
	                               "", "", "{tleave}", NULL);
	addLanguageTagMultiTableRegex (language, "structbody",
	                               "^\\{",
	                               "", "", "{tenter=structbody}", NULL);
	addLanguageTagMultiTableRegex (language, "structbody",
	                               "^/\\*",
	                               "", "", "{tenter=comment}", NULL);
	addLanguageTagMultiTableRegex (language, "structbody",
	                               "^.",
	                               "", "", "", NULL);
}

extern parserDefinition* GPerfParser (void)
{
	static const char *const extensions [] = {
		"perf",
		"gperf",
		NULL
	};

	static const char *const aliases [] = {
		NULL
	};

	static const char *const patterns [] = {
		NULL
	};

	static kindDefinition GPerfKindTable [] = {
		{
		  true, 'k', "keyword", "keywords",
		},
		{
		  true, 'h', "hfunc", "hash function names",
		},
		{
		  true, 'l', "lfunc", "lookup function names",
		},
		{
		  true, 'c', "class", "class names",
		},
		{
		  true, 's', "strpool", "string pool names",
		},
	};

	parserDefinition* const def = parserNew ("GPerf");

	def->versionCurrent= 0;
	def->versionAge    = 0;
	def->enabled       = true;
	def->extensions    = extensions;
	def->patterns      = patterns;
	def->aliases       = aliases;
	def->method        = METHOD_NOT_CRAFTED|METHOD_REGEX;
	def->useCork       = CORK_QUEUE;
	def->kindTable     = GPerfKindTable;
	def->kindCount     = ARRAY_SIZE(GPerfKindTable);
	def->initialize    = initializeGPerfParser;

	return def;
}
