#include "listservice.h"

ListService::ListService()
{

}

ListService::~ListService()
{

}

void ListService::command(QString str) {
    emit receiveListing(str,"str2");
}
