#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int MAXBUF = 500;

char *concat(char *str1, char *str2)
{
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	int len = len1 + len2 + 1;
	int i;
	int k;
	char newstr[len];

	for (i = 0; i < len1; i++)
		newstr[i] = str1[i];

	for (k = 0; k < len2; i++, k++)
		newstr[i] = str2[k];

	newstr[i] = '\0';

	return strdup(newstr);
}

char *threeconcat(char *str1, char *str2, char *str3)
{
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	int len3 = strlen(str3);
	int len = len1 + len2 + len3 + 1;
	int i;
	int k;
	int j;
	char newstr[len];

	for (i = 0; i < len1; i++)
		newstr[i] = str1[i];

	for (k = 0; k < len2; i++, k++)
		newstr[i] = str2[k];

	for (j = 0; j < len3; i++, j++)
		newstr[i] = str3[j];

	newstr[i] = '\0';

	return strdup(newstr);
}

char *removePath(char *filename)
{
	int i;
	int len = strlen(filename);
	char clean[len];

	for (i = 0; filename[i] != '.' && i < len; i++)
		clean[i] = filename[i];

	for (; i < len; i++)
		clean[i] = '\0';

	return strdup(clean);
}


void copyFile(char *oldpath, char *newpath)
{
	FILE *oldfile = fopen(oldpath, "rb");
	printf("I made it this far\n");
	FILE *newfile = fopen(newpath, "wb");
	char buf[MAXBUF];
	char *line = NULL;
	printf("Party hard\n");

	while (line = fgets(buf, MAXBUF, oldfile))
		fprintf(newfile, "%s", line);

	fclose(oldfile);
	fclose(newfile);

	return;
}

copyAllNewFiles(char **origfiles, char **newfiles)
{
	int i;
	char buf[MAXBUF];
	char *line = NULL;

	for (i = 0; i < 7; i++){
		FILE *origfile = fopen(origfiles[i], "rb");
		FILE *newfile = fopen(newfiles[i], "wb");

		while(line = fgets(buf, MAXBUF, origfile))
			fprintf(newfile, "%s", line);

		fclose(origfile);
		fclose(newfile);
	}

	return;
}

copyAllOldFiles(char **origfiles, char **oldfiles)
{
	int i;
	char buf[MAXBUF];
	char *line = NULL;

	for (i = 0; i < 7; i++){
		FILE *origfile = fopen(origfiles[i], "rb");
		FILE *oldfile = fopen(oldfiles[i], "wb");

		while(line = fgets(buf, MAXBUF, origfile))
			fprintf(oldfile, "%s", line);

		fclose(origfile);
		fclose(oldfile);
	}

	return;
}

char **makeNewFilenames(char *newpath, char *cleancor, char **bases)
{
	char **newfiles = malloc(sizeof(char *) * 7);
	int i;

	for (i = 0; i < 7; i++)
		newfiles[i] = threeconcat(newpath, cleancor, bases[i]);

	return newfiles;

}

char **makeOldFilenames(char *oldpath, char *cleancor, char **bases)
{
	char **oldfiles = malloc(sizeof(char *) * 7);
	int i;

	for (i = 0; i < 7; i++)
		oldfiles[i] = threeconcat(oldpath, cleancor, bases[i]);

	return oldfiles;

}

char **makeOrigFilenames(char *origpath, char **rootlessfiles)
{
	char **origfiles = malloc(sizeof(char *) * 7);
	int i;

	for (i = 0; i < 7; i++)
		origfiles[i] = concat(origpath, rootlessfiles[i]);

	return origfiles;

}

char **makeRootlessFilenames(char *cleancor, char **bases)
{
	char **rootlessfiles = malloc(sizeof(char *) * 7);
	int i;

	for (i = 0; i < 7; i++)
		rootlessfiles[i] = concat(cleancor, bases[i]);

	return rootlessfiles;
}

char **makeFileBases()
{
	char **files = malloc(sizeof (char *) * 7);

	files[0] = "_AffixToSigs.txt";
	files[1] = "_mostFreqWordsNotInSigs.txt";
	files[2] = "_SigToStems.txt";
	files[3] = "_StemToWords.txt";
	files[4] = "_WordsInSigs.txt";
	files[5] = "_WordsNotinSigs.txt";
	files[6] = "_WordToSigs.txt";

	return files;
}

char *makeCmd(char *lang, char *corpus, char *datasets)
{
	char *cmd1 = threeconcat("python3 lxa5.py --language=", lang, " --corpus=");
	char *cmd2 = threeconcat(cmd1, corpus, " --datafolder=");
	char *cmd = concat(cmd2, datasets);

	return strdup(cmd);
}

int checkdiff(FILE *oldfile, FILE *newfile, FILE *diffsfile, FILE *fulldiffs)
{
	char *nline = NULL;
	char *oline = NULL;
	char buf1[MAXBUF];
	char buf2[MAXBUF];
	int tot = 0;
	int linecount = 1;

	while ((nline = fgets(buf1, MAXBUF, newfile))
		&& (oline = fgets(buf2, MAXBUF, oldfile))){
		if (strcmp(nline, oline)){
			fprintf(diffsfile, "old %d: %s", linecount, oline);
			fprintf(diffsfile, "new %d: %s\n", linecount, nline);
			fprintf(fulldiffs, "***old %d: %snew %d: %s\n", linecount, oline, linecount, nline);
			tot ++;
		} else {
			fprintf(fulldiffs, "old %d: %snew %d: %s\n", linecount, oline, linecount, nline);
		}
		linecount++;
	}

	while (oline = fgets(buf2, MAXBUF, oldfile)){
		fprintf(diffsfile, "old %d: %s", linecount, oline);
		fprintf(diffsfile, "new: \n\n");
		fprintf(fulldiffs, "***old %d: %snew %d: %s\n", linecount, oline, linecount, nline);
		tot++;
		linecount++;
	}

	while (nline = fgets(buf1, MAXBUF, newfile)){
		fprintf(diffsfile, "old: \n");
		fprintf(diffsfile, "new %d: %s \n", linecount, nline);
		fprintf(fulldiffs, "***old %d: %snew %d: %s\n", linecount, oline, linecount, nline);
		tot++;
		linecount++;
	}

	fprintf(diffsfile, "Number of differences: %d\n\n", tot);

	return tot;
}


int checkdiffs(char **rootlessfiles, char **oldfiles, char **newfiles)
{
	int i;
	FILE *diffsfile = fopen("regtest/diffs.txt", "wb");
	FILE *fulldiffs = fopen("regtest/fulldiffs.txt", "wb");
	int tot = 0;

	for (i = 0; i < 7; i++){
		fprintf(diffsfile, "%s:\n", rootlessfiles[i]);
		FILE *oldf = fopen(oldfiles[i], "rb");
		FILE *newf = fopen(newfiles[i], "rb");
		tot += checkdiff(oldf, newf, diffsfile, fulldiffs);
		fclose(oldf);
		fclose(newf);
	}

	fprintf(diffsfile, "\n\nTotal number of differences: %d", tot);
	fclose(diffsfile);
	fclose(fulldiffs);
	return tot;
}

int main(int argc, char *argv[])
{
	if (argc != 4){
		fprintf(stderr,"Please enter a language, the corpus name, and the path to the datasets folder\n");
		exit(1);
	}

	char *language = argv[1];
	char *corpus = argv[2];
	char *datasets = argv[3];
	DIR *regtestdir;
	DIR *olddir;
	DIR *newdir;
	struct stat s;

    mkdir("regtest", 0700);
    mkdir("regtest/old", 0700);
    mkdir("regtest/new", 0700);
    //regtestdir = opendir("regtest");
    //olddir = opendir("regtest/old");
    //newdir = opendir("regtest/new");

    int len = strlen(datasets) - 1;
    if (datasets[len] != '/')
    	datasets = concat(datasets, "/");
	char *origpath = threeconcat(datasets, language, "/lxa/");
	char *cleancor = removePath(corpus);
	char *oldpath = "regtest/old/";
	char *newpath = "regtest/new/";

	char **filebases = makeFileBases();
	char **rootlessfiles = makeRootlessFilenames(cleancor, filebases);
	char **origfiles = makeOrigFilenames(origpath, rootlessfiles);
	char **oldfiles = makeOldFilenames(oldpath, cleancor, filebases);
	char **newfiles = makeNewFilenames(newpath, cleancor, filebases);

	copyAllOldFiles(origfiles, oldfiles);

	char *cmd = makeCmd(language, corpus, datasets);
	system(cmd);

	copyAllNewFiles(origfiles, newfiles);

	int tot;
	tot = checkdiffs(rootlessfiles, oldfiles, newfiles);
	fprintf(stdout, "\nTotal number of differences: %d\n", tot);

    return 0;
}