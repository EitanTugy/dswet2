/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Spring 2019                                          */
/* Homework : Wet 2                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main2.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library2.h"
#include <iostream>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

/* The command's strings */
typedef enum {
    NONE_CMD = -2,
    COMMENT_CMD = -1,
    INIT_CMD = 0,
    ADD_ROOM_CMD = 1,
    DEL_ROOM_CMD = 2,
    ADD_LECTURE_CMD = 3,
    DELETE_LECTURE_CMD = 4,
    MERGE_COURSES_CMD = 5,
    COMPETITION_CMD = 6,
    GET_AVERAGE_CMD = 7,
    QUIT_CMD = 8
} commandType;

static const int numActions = 9;
static const char *commandStr[] = {
        "Init",
        "addRoom",
        "deleteRoom",
        "addLecture",
        "deleteLecture",
        "mergeCourses",
        "competition",
        "getAverageStudentsInCourse",
        "Quit" };

static const char* ReturnValToStr(int val) {
    switch (val) {
        case SUCCESS:
            return "SUCCESS";
        case ALLOCATION_ERROR:
            return "ALLOCATION_ERROR";
        case FAILURE:
            return "FAILURE";
        case INVALID_INPUT:
            return "INVALID_INPUT";
        default:
            return "";
    }
}

/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum {
    error_free, error
} errorType;
static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString); return error; }

static bool isInit = false;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv) {
    char buffer[MAX_STRING_INPUT_SIZE];

    // Reading commands
    while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
        fflush(stdout);
        if (parser(buffer) == error)
            break;
    };
    return 0;
}

/***************************************************************************/
/* Command Checker                                                         */
/***************************************************************************/

static commandType CheckCommand(const char* const command,
                                const char** const command_arg) {
    if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
        return (NONE_CMD);
    if (StrCmp("#", command)) {
        if (strlen(command) > 1)
            printf("%s", command);
        return (COMMENT_CMD);
    };
    for (int index = 0; index < numActions; index++) {
        if (StrCmp(commandStr[index], command)) {
            *command_arg = command + strlen(commandStr[index]) + 1;
            return ((commandType) index);
        };
    };
    return (NONE_CMD);
}

/***************************************************************************/
/* Commands Functions                                                      */
/***************************************************************************/

static errorType OnInit(void** DS, const char* const command);
static errorType OnAddRoom(void* DS, const char* const command);
static errorType OnDeleteRoom(void* DS, const char* const command);
static errorType OnAddLecture(void* DS, const char* const command);
static errorType OnDeleteLecture(void* DS, const char* const command);
static errorType OnMergeCourses(void* DS, const char* const command);
static errorType OnCompetition(void* DS, const char* const command);
static errorType OnGetAverageStudentsInCourse(void* DS, const char* const command);
static errorType OnQuit(void** DS, const char* const command);

/***************************************************************************/
/* Parser                                                                  */
/***************************************************************************/

static errorType parser(const char* const command) {
    static void *DS = NULL; /* The general data structure */
    const char* command_args = NULL;
    errorType rtn_val = error;

    commandType command_val = CheckCommand(command, &command_args);

    switch (command_val) {

        case (INIT_CMD):
            rtn_val = OnInit(&DS, command_args);
            break;
        case (ADD_ROOM_CMD):
            rtn_val = OnAddRoom(DS, command_args);
            break;
        case (DEL_ROOM_CMD):
            rtn_val = OnDeleteRoom(DS, command_args);
            break;
        case (ADD_LECTURE_CMD):
            rtn_val = OnAddLecture(DS, command_args);
            break;
        case (DELETE_LECTURE_CMD):
            rtn_val = OnDeleteLecture(DS, command_args);
            break;
        case (MERGE_COURSES_CMD):
            rtn_val = OnMergeCourses(DS, command_args);
            break;
        case (COMPETITION_CMD):
            rtn_val = OnCompetition(DS, command_args);
            break;
        case (GET_AVERAGE_CMD):
            rtn_val = OnGetAverageStudentsInCourse(DS, command_args);
            break;
        case (QUIT_CMD):
            rtn_val = OnQuit(&DS, command_args);
            break;
        case (COMMENT_CMD):
            rtn_val = error_free;
            break;
        case (NONE_CMD):
            rtn_val = error;
            break;
        default:
            assert(false);
            break;
    };
    return (rtn_val);
}

/***************************************************************************/
/* OnInit                                                                  */
/***************************************************************************/
static errorType OnInit(void** DS, const char* const command) {
    if (isInit) {
        printf("Init was already called.\n");
        return (error_free);
    };
    isInit = true;
    int n;
    ValidateRead(sscanf(command, "%d", &n), 1, "Init failed.\n");
    *DS = Init(n);
    if (*DS == NULL) {
        printf("Init failed.\n");
        return error;
    };
    printf("Init done.\n");

    return error_free;
}


/***************************************************************************/
/* OnAddRoom                                                          */
/***************************************************************************/
static errorType OnAddRoom(void* DS, const char* const command) {
    int roomID;
    ValidateRead(sscanf(command, "%d", &roomID), 1, "addRoom failed.\n");
    StatusType res = addRoom(DS, roomID);

    if (res != SUCCESS) {
        printf("addRoom: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("addRoom: %s\n", ReturnValToStr(res));
    return error_free;
}


/***************************************************************************/
/* OnDeleteRoom                                                          */
/***************************************************************************/
static errorType OnDeleteRoom(void* DS, const char* const command) {
    int roomID;
    ValidateRead(sscanf(command, "%d", &roomID), 1, "deleteRoom failed.\n");
    StatusType res = deleteRoom(DS, roomID);

    if (res != SUCCESS) {
        printf("deleteRoom: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("deleteRoom: %s\n", ReturnValToStr(res));
    return error_free;
}


/***************************************************************************/
/* OnAddLecture                                                             */
/***************************************************************************/
static errorType OnAddLecture(void* DS, const char* const command) {
    int courseID;
    int groupID;
    int roomID;
    int hour;
    int numStudents;
    ValidateRead(sscanf(command, "%d %d %d %d %d", &courseID, &groupID, &roomID, &hour, &numStudents), 5, "addLecture failed.\n");
    StatusType res = addLecture(DS, courseID, groupID, roomID, hour, numStudents);

    if (res != SUCCESS) {
        printf("addLecture: %s\n", ReturnValToStr(res));
        return error_free;
    } else {
        printf("addLecture: %s\n", ReturnValToStr(res));
    }

    return error_free;
}


/***************************************************************************/
/* OnDeleteLecture                                                            */
/***************************************************************************/
static errorType OnDeleteLecture(void* DS, const char* const command) {
    int hour;
    int roomID;
    ValidateRead(sscanf(command, "%d %d", &hour, &roomID), 2, "deleteLecture failed.\n");
    StatusType res = deleteLecture(DS, hour, roomID);
    if (res != SUCCESS) {
        printf("deleteLecture: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("deleteLecture: %s\n", ReturnValToStr(res));
    return error_free;
}

/***************************************************************************/
/* OnMergeCourses                                                            */
/***************************************************************************/
static errorType OnMergeCourses(void* DS, const char* const command) {
    int courseID1, courseID2;
    ValidateRead(sscanf(command, "%d %d", &courseID1, &courseID2), 2,
                 "mergeCourses failed.\n");
    StatusType res = mergeCourses(DS, courseID1, courseID2);

    if (res != SUCCESS) {
        printf("mergeCourses: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("mergeCourses: %s\n", ReturnValToStr(res));
    return error_free;
}


/***************************************************************************/
/* OnCompetition                                                         */
/***************************************************************************/
static errorType OnCompetition(void* DS, const char* const command) {
    int courseID1, courseID2, numGroups, winner;
    ValidateRead(sscanf(command, "%d %d %d", &courseID1, &courseID2, &numGroups), 3, "competition failed.\n");
    StatusType res = competition(DS, courseID1, courseID2, numGroups, &winner);

    if (res != SUCCESS) {
        printf("competition: %s\n", ReturnValToStr(res));
        return error_free;
    }

    cout << "Course with id " << winner << " won" << endl;
    return error_free;
}


/***************************************************************************/
/* OnGetAverageStudentsInCourse                                                         */
/***************************************************************************/
static errorType OnGetAverageStudentsInCourse(void* DS, const char* const command) {
    int hour;
    int roomID;
    float average;
    ValidateRead(sscanf(command, "%d %d", &hour, &roomID), 2, "getAverageStudentsInCourse failed.\n");
    StatusType res = getAverageStudentsInCourse(DS, hour, roomID, &average);

    if (res != SUCCESS) {
        printf("getAverageStudentsInCourse: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("The average is: %.2f\n", average);
    return error_free;
}



/***************************************************************************/
/* OnQuit                                                                  */
/***************************************************************************/
static errorType OnQuit(void** DS, const char* const command) {
    Quit(DS);
    if (*DS != NULL) {
        printf("Quit failed.\n");
        return error;
    };

    isInit = false;
    printf("Quit done.\n");

    return error_free;
}

#ifdef __cplusplus
}
#endif