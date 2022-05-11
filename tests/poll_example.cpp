/*
 * If not stated otherwise in this file or this component's LICENSE
 * file the following copyright and licenses apply:
 *
 * Copyright (c) 2022 Nuuday.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/**
 *
 *
 *
 */
#define NUM_FSELECT_FILES	1

int main(int argc, char *argv[]) {
    char buf[8];
    int f;

    if (argc != 2) {
        fprintf(stderr, "Usage %s <file_name> \n", argv[0]);
        return -1;
    }

    char* fbPath = argv[1];

	int fds[NUM_FSELECT_FILES];

    fds[0] = open(fbPath, O_RDONLY);

    int  nfds = fds[NUM_FSELECT_FILES - 1] + 1;

    if (fds < 0)  {
        perror("failed to open file");
        return 1;
    }

	for(int tries=0; tries < 16; tries++) {
	    static char buf[256];
		fd_set rfds;
		int res;

		FD_ZERO(&rfds);
		for (int i = 0; i < NUM_FSELECT_FILES; i++) {
			FD_SET(fds[i], &rfds);
        }

		res = select(nfds, &rfds, NULL, NULL, NULL);

		if (res < 0) {
			perror("Calling select failed");
			return 1;
		}

		for (int i = 0; i < NUM_FSELECT_FILES; i++) {
			if (!FD_ISSET(fds[i], &rfds)) {
				continue;
			}

            lseek(fds[i], 0, SEEK_SET);

			res = read(fds[i], buf, sizeof(buf));
			if (res < 0) {
				perror("Failed to read data");
				return 1;
			}
			printf("Got value: %s\n", buf);
		}
	}
	return 0;
}
