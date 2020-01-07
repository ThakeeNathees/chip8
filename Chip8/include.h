#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdlib>

#include <stdio.h>
#include <dirent.h>


#ifndef __MINGW32__
#define stderr stderr_s
#define strerror strerror_s
#endif

inline std::string toHexString(unsigned int value, unsigned int fill = 2) {
	std::string hexStr;
	std::stringstream sstream;
	sstream << std::setfill('0') << std::setw(fill) << std::hex << (unsigned int)value;
	hexStr = sstream.str();
	sstream.clear();
	return hexStr;
}

inline  void list_directory( const char* dirname)
{
	struct dirent** files;
	int i;
	int n;

	/* Scan files in directory */
	n = scandir(dirname, &files, NULL, alphasort);
	if (n >= 0) {

		/* Loop through file names */
		for (i = 0; i < n; i++) {
			struct dirent* ent;

			/* Get pointer to file entry */
			ent = files[i];

			/* Output file name */
			switch (ent->d_type) {
			case DT_REG:
				printf("%s\n", ent->d_name);
				break;

			case DT_DIR:
				printf("%s/\n", ent->d_name);
				break;

			case DT_LNK:
				printf("%s@\n", ent->d_name);
				break;

			default:
				printf("%s*\n", ent->d_name);
			}

		}

		/* Release file names */
		for (i = 0; i < n; i++) {
			free(files[i]);
		}
		free(files);

	}
	else {
		printf("Cannot open %s\n", dirname);
		exit(EXIT_FAILURE);
	}
}

#define print(x) std::cout<< x << std::endl;
