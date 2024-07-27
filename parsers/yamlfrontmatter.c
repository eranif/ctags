/*
*
*   Copyright (c) 2022, Masatake YAMATO
*   Copyright (c) 2022, Red Hat, K.K.
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License version 2 or (at your option) any later version.
*
* This module contains functions for extracting language objects in FrontMatter
* using Yaml.
*
* https://gohugo.io/content-management/front-matter
*/

/*
*   INCLUDE FILES
*/
#include "general.h"	/* must always come first */

#include "frontmatter.h"
#include "yaml.h"

#include "entry.h"
#include "parse.h"
#include "read.h"
#include "subparser.h"


/*
*   DATA DECLARATIONS
*/
struct sYamlFrontMatterSubparser {
	yamlSubparser yaml;
};


/*
*   FUNCTION PROTOTYPES
*/
static bool yamlFrontmattterInitTagEntry (tagEntryInfo *e, yamlSubparser *yaml, char *name, void *data);


/*
*   DATA DEFINITIONS
*/

static langType frontMatterLang;

static tagYpathTable ypathTables [] = {
	{
		"title",
		YPATH_DSTAT_LAST_VALUE,
		.initTagEntry = yamlFrontmattterInitTagEntry,
	},
};


/*
*   FUNCTION DEFINITIONS
*/

static bool yamlFrontmattterInitTagEntry (tagEntryInfo *e, yamlSubparser *s CTAGS_ATTR_UNUSED,
										  char *name, void * data CTAGS_ATTR_UNUSED)
{
	initForeignTagEntry (e, name, frontMatterLang, FRONTMATTER_TITLE_KIND);
	return true;
}

static void findYamlFrontMatterTags (void)
{
	scheduleRunningBaseparser (0);
}

static void yamlFrontMatterInitialize (langType language)
{
	frontMatterLang = getNamedLanguage ("FrontMatter", 0);
}

extern parserDefinition* YamlFrontMatter (void)
{
	static struct sYamlFrontMatterSubparser yamlfrontmatterSubparser = {
		.yaml = {
			.subparser = {
				.direction = SUBPARSER_SUB_RUNS_BASE,
			},
			.ypathTables = ypathTables,
			.ypathTableCount = ARRAY_SIZE (ypathTables),
		},
	};
	static parserDependency dependencies [] = {
		{ DEPTYPE_SUBPARSER, "Yaml", &yamlfrontmatterSubparser },
		{ DEPTYPE_FOREIGNER, "FrontMatter", NULL },
	};

	parserDefinition* const def = parserNew ("YamlFrontMatter");

	def->dependencies = dependencies;
	def->dependencyCount = ARRAY_SIZE (dependencies);

	def->kindTable	= NULL;
	def->kindCount = 0;
	def->parser	= findYamlFrontMatterTags;
	def->initialize = yamlFrontMatterInitialize;

	/* This parser runs ONLY as a part of FrontMatter parser.
	 * User may not want to enable/disable this parser directly. */
	def->invisible = true;

	return def;
}
