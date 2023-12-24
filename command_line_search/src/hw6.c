#include "hw6.h"

char *char1, *char2;
int num1 = 0, num2 = 0;

int input_file(int argc, char *argv[]) {
    FILE *fp = fopen(argv[argc-2], "r");
    if(fp == NULL) {
        return 1;
    }
    fclose(fp);
    return 0;
}

int output_file(int argc, char *argv[]) {
    FILE *fp = fopen(argv[argc-1], "w");
    if(fp == NULL) {
        return 1;
    }
    fclose(fp);
    return 0;
}

int valid_l(char* lvalue) {
    char1 = strtok(lvalue, ",");
    char2 = strtok(NULL, ",");

    if(char1 == NULL || char2 == NULL) {
        return 1;
    }

    num1 = strtol(char1, NULL, 10);
    num2 = strtol(char2, NULL, 10);

    if(num1>num2) {
        return 1;
    }

    return 0;
}

int wildcard(char *svalue) {
    int beginning = 0, end = 0;
    if (svalue[0] == '*') {
        beginning = 1;
    } else {
        beginning = 0;
    }

    char *temp = svalue;
    while(*(temp+1) != '\0') {
        temp = temp+1;
    }
    if(*temp == '*') {
        end = 1;
    } else {
        end = 0;
    }

    if((beginning && end) || (beginning == 0 && end == 0)) {
        return 1;
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc < 7) {
        return MISSING_ARGUMENT;
    }

    extern char *optarg;
    extern int optind;
    int c = 0;
    int repeats = 0, repeatr = 0, repeatl = 0, repeatw = 0;
    char *svalue, *rvalue, *lvalue;

    while((c = getopt(argc, argv, "s:r:l:w")) != -1) {
        switch (c) {
            case 's':
                if (repeats > 1) {
                    break;
                }
                svalue = optarg;
                repeats++;
                break; 
            case 'r': 
                if (repeatr > 1) {
                    break;
                }
                rvalue = optarg;
                repeatr++;
                break; 
            case 'l':
                if (repeatl > 1) {
                    break;
                }
                lvalue = optarg;
                repeatl++;
                break; 
            case 'w':
                if (repeatw > 1) {
                    break;
                }
                repeatw++;
                break;
            case '?':
                break;
        }
    }
    // TESTS DUPLICATE_ARGUMENT(8)
    if(repeats > 1 || repeatr > 1 || repeatl > 1 || repeatw > 1) {
        return DUPLICATE_ARGUMENT;
    }
    // TESTS INPUT_FILE_MISSING(2)
    if(input_file(argc, argv)) {
        return INPUT_FILE_MISSING;
    }
    // TEST OUTPUT_FILE_UNWRITABLE(3)
    if(output_file(argc, argv)) {
        return OUTPUT_FILE_UNWRITABLE;
    }
    // TEST S_ARGUMENT_MISSING(4)
    if((repeats == 0) || (*svalue == '-')) {
        return S_ARGUMENT_MISSING;
    }
    // TEST R_ARGUMENT_MISSING(5)
    if((repeatr == 0) || (*rvalue == '-')) {
        return R_ARGUMENT_MISSING;
    }
    // TEST L_ARGUMENT_INVALID(6)
    if((repeatl >=1 && *lvalue == '-') || (repeatl >= 1 && valid_l(lvalue))) {
        return L_ARGUMENT_INVALID;
    }
    //TEST WILDCARD_INVALID(7)
    if(repeatw && wildcard(svalue)) {
        return WILDCARD_INVALID;
    }

    int search_length = 0;


    FILE *input_file = fopen(argv[argc-2], "r");
    FILE *output_file = fopen(argv[argc-1], "w");
    char line[MAX_LINE];
    char *found_text = NULL;

    int line_counter = 0;
    while(!feof(input_file)) {
        line_counter++;
        fgets(line, MAX_LINE, input_file);
        if((strstr(line, svalue) == NULL && repeatw == 0) || (line_counter < num1 && repeatl == 1) || (line_counter > num2 && repeatl ==1)) {
            fprintf(output_file, "%s", line);
        } else if(repeatw == 1) { //if wildcard arguement is present
            char search[strlen(svalue)-1];
            if(svalue[strlen(svalue)-1] == '*') { //validates "foo*"
                char *tempr = line;
                strcpy(search, svalue);
                search[strlen(search)-1] = '\0';
                while(*tempr != '\0') {
                    int breaking = 0;
                    if(*tempr == search[0]) {
                        for(int i = 0; i < (int) strlen(search); i++) {
                            if(tempr[i] != search[i]) {
                                breaking = 1;
                                break;
                            }
                        }
                        if(!(tempr == line) && !ispunct(tempr[-1]) && !isspace(tempr[-1])) {
                            breaking = 1;
                        }
                        if(!breaking) {
                            for(int i = 0; i < (int) strlen(rvalue); i++) {
                                fputc(rvalue[i], output_file);
                            }

                            int wildcard_length = 0;
                            while(!ispunct(*(tempr+wildcard_length)) && !isspace(*(tempr+wildcard_length)) && (*(tempr+wildcard_length)) != '\0') {
                                wildcard_length++;
                            }
                            tempr += wildcard_length;
                            continue;
                        }
                    } 
                    fputc(*tempr, output_file);
                    tempr = tempr + 1;
                }
            } else { //validates "*foo"
                char *tempr = line, *index = line, *holder = line;
                strcpy(search, svalue+1);
                
                while(*tempr != '\0') {
                    int breaking = 0;
                    if(*tempr == search[0]) {
                        for(int i = 0; i < (int) strlen(search); i++) {
                            if(tempr[i] != search[i]) {
                                breaking = 1;
                                break;
                            }
                        }

                        if(!breaking) {
                            if(tempr[strlen(search)] == '\n' || tempr[strlen(search)] == '\0' || ispunct(tempr[strlen(search)]) || isspace(tempr[strlen(search)])) {
                                index = tempr;
                                while(!(index == holder) && !ispunct(*index) && !isspace(*index)) {
                                    index = index-1;
                                }
                                if(!(index == holder)) {
                                    index++;
                                }

                                for(char *i = holder; i < index; i++) {
                                    fputc(*i, output_file);
                                }
                                for(int i = 0; i < (int) strlen(rvalue); i++) {
                                    fputc(rvalue[i], output_file);
                                }
                                holder = tempr+strlen(search);
                                tempr += strlen(search);
                                continue;
                            }
                        }
                    }
                    tempr = tempr + 1;
                }
                fprintf(output_file, "%s", holder);
            }
        } 
        else { //if no wildcard
            int index = 0;
            char temp[MAX_LINE];
            while((found_text = strstr(line, svalue))) {
                search_length = strlen(svalue);
                strcpy(temp, line);
                index = found_text - line;
                line[index] = '\0';
                strcat(line, rvalue);
                strcat(line, temp+index+search_length);
            }
            fprintf(output_file, "%s", line);
        }
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}