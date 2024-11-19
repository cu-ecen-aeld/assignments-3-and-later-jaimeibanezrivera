#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // Open syslog for logging
    openlog("writer", LOG_PID, LOG_USER);

    // Check if two arguments are provided (file path and content)
    if (argc != 3) {
        syslog(LOG_ERR, "Error: Two arguments are required.");
        printf("Error: Two arguments are required.\n");
        return 1;
    }

    char *writefile = argv[1];
    char *writestr = argv[2];

    // Debug: Print file and content information
    printf("Attempting to write to file: %s\n", writefile);
    printf("Content to write: %s\n", writestr);

    // Attempt to open the file for writing
    FILE *file = fopen(writefile, "w");
    if (file == NULL) {
        // Log and print the error
        syslog(LOG_ERR, "Error: Could not open file %s for writing: %s", writefile, strerror(errno));
        printf("Error: Could not open file %s for writing: %s\n", writefile, strerror(errno));
        return 1;
    }

    // Log successful writing
    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);
    printf("Writing %s to %s\n", writestr, writefile);

    // Write the string to the file
    if (fprintf(file, "%s", writestr) < 0) {
        syslog(LOG_ERR, "Error: Could not write to file %s: %s", writefile, strerror(errno));
        printf("Error: Could not write to file %s: %s\n", writefile, strerror(errno));
        fclose(file);
        return 1;
    }

    // Close the file
    if (fclose(file) != 0) {
        syslog(LOG_ERR, "Error: Could not close file %s: %s", writefile, strerror(errno));
        printf("Error: Could not close file %s: %s\n", writefile, strerror(errno));
        return 1;
    }

    // Final success message
    printf("File written successfully: %s\n", writefile);
    closelog();
    return 0;
}
