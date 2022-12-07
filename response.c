#include <fastdb.h>

void sendNotFoundResponse()
{
    printf("{"
           "    \"response\": %i"
           "}",
           NOT_FOUND_RESPONSE);
}

void sendIncorrectFormatResponse()
{
    printf("{"
           "    \"response\": %i"
           "}",
           INCORRECT_REQUEST_FORMAT);
}
void sendSucessResponse(char *data)
{
    printf("{"
           "    \"response\": %i,"
           "    \"data\": \"%s\""
           "}",
           SUCESSFUL_RESPONSE,
           data);
}
void sendOtherErroResponse(char *res)
{
    printf("{"
           "    \"response\": %i,"
           "    \"data\": \"%s\""
           "}",
           OTHER_ERROR_RESPONSE,
           res);
}