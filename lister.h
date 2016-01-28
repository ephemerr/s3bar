#ifndef LISTER_H
#define LISTER_H

#include <libs3.h>
#include <QtCore>

extern int statusG;

typedef struct list_bucket_callback_data
{
    int isTruncated;
    char nextMarker[1024];
    int keyCount;
    int allDetails;
} list_bucket_callback_data;

void S3_init();
int should_retry();
void printError();
S3Status responsePropertiesCallback
    (const S3ResponseProperties *properties, void *callbackData);
void responseCompleteCallback(S3Status status,
                                     const S3ErrorDetails *error,
                                     void *callbackData);
void list_bucket(const char *bucketName, const char *prefix,
                        const char *marker, const char *delimiter,
                        int maxkeys,  const char* ak, const char* sk);
S3Status listBucketCallback(int isTruncated, const char *nextMarker,
                                   int contentsCount,
                                   const S3ListBucketContent *contents,
                                   int commonPrefixesCount,
                                   const char **commonPrefixes,
                                   void *callbackData);

QString getName(int i);
QString getSize(int i);
QString getDate(int i);
int     getCount();


#endif // LISTER_H

