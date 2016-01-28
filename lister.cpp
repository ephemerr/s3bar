#include "lister.h"
#include "panelentry.h"
#include <time.h>
#include <QtCore>

int statusG = 0;
static char errorDetailsG[4096] = { 0 };

static QList<S3ListBucketContent> savedContents;
static QList<QString> savedCommonPrefixes;
static QList<QString> savedKeys;

void S3_init()
{
    S3Status status;
    const char *hostname = getenv("S3_HOSTNAME");

    if ((status = S3_initialize("s3", S3_INIT_ALL, hostname))
        != S3StatusOK) {
        fprintf(stderr, "Failed to initialize libs3: %s\n",
                S3_get_status_name(status));
        return;
    }
}


int should_retry()
{
    static int retriesG = 5;

    if (retriesG--) {
        static int retrySleepInterval = 1;
        struct timespec ts =  {retrySleepInterval,0};
        // Sleep before next retry; start out with a 1 second sleep
        nanosleep(&ts, NULL);
        // Next sleep 1 second longer
        retrySleepInterval++;
        return 1;
    }

    return 0;
}

void printError()
{
    if (statusG < S3StatusErrorAccessDenied) {
        fprintf(stderr, "\nERROR: %s\n", S3_get_status_name((S3Status)statusG));
    }
    else {
        fprintf(stderr, "\nERROR: %s\n", S3_get_status_name((S3Status)statusG));
        fprintf(stderr, "%s\n", errorDetailsG);
    }
}

// This callback does the same thing for every request type: prints out the
// properties if the user has requested them to be so
S3Status responsePropertiesCallback
    (const S3ResponseProperties *properties, void *callbackData)
{
    (void) callbackData;
    (void) properties;

    return S3StatusOK;
}

// This callback does the same thing for every request type: saves the status
// and error stuff in global variables
void responseCompleteCallback(S3Status status,
                                     const S3ErrorDetails *error,
                                     void *callbackData)
{
    (void) callbackData;

    statusG = status;
    // Compose the error details message now, although we might not use it.
    // Can't just save a pointer to [error] since it's not guaranteed to last
    // beyond this callback
    int len = 0;
    if (error && error->message) {
        len += snprintf(&(errorDetailsG[len]), sizeof(errorDetailsG) - len,
                        "  Message: %s\n", error->message);
    }
    if (error && error->resource) {
        len += snprintf(&(errorDetailsG[len]), sizeof(errorDetailsG) - len,
                        "  Resource: %s\n", error->resource);
    }
    if (error && error->furtherDetails) {
        len += snprintf(&(errorDetailsG[len]), sizeof(errorDetailsG) - len,
                        "  Further Details: %s\n", error->furtherDetails);
    }
    if (error && error->extraDetailsCount) {
        len += snprintf(&(errorDetailsG[len]), sizeof(errorDetailsG) - len,
                        "%s", "  Extra Details:\n");
        int i;
        for (i = 0; i < error->extraDetailsCount; i++) {
            len += snprintf(&(errorDetailsG[len]),
                            sizeof(errorDetailsG) - len, "    %s: %s\n",
                            error->extraDetails[i].name,
                            error->extraDetails[i].value);
        }
    }
}

S3Status listBucketCallback(int isTruncated, const char *nextMarker,
                                   int contentsCount,
                                   const S3ListBucketContent *contents,
                                   int commonPrefixesCount,
                                   const char **commonPrefixes,
                                   void *callbackData)
{
    list_bucket_callback_data *data =
        (list_bucket_callback_data *) callbackData;

    data->isTruncated = isTruncated;
    // This is tricky.  S3 doesn't return the NextMarker if there is no
    // delimiter.  Why, I don't know, since it's still useful for paging
    // through results.  We want NextMarker to be the last content in the
    // list, so set it to that if necessary.
    if ((!nextMarker || !nextMarker[0]) && contentsCount) {
        nextMarker = contents[contentsCount - 1].key;
    }
    if (nextMarker) {
        snprintf(data->nextMarker, sizeof(data->nextMarker), "%s",
                 nextMarker);
    }
    else {
        data->nextMarker[0] = 0;
    }

//    if (contentsCount && !data->keyCount) {
//        printListBucketHeader(data->allDetails);
//    }

    int i;
    for (i = 0; i < contentsCount; i++) {
        const S3ListBucketContent *content = &(contents[i]);
        char timebuf[256];
        time_t t = (time_t) content->lastModified;
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%dT%H:%M:%SZ",
                 gmtime(&t));
        char sizebuf[16];
        if (content->size < 100000) {
            sprintf(sizebuf, "%5llu", (unsigned long long) content->size);
        }
        else if (content->size < (1024 * 1024)) {
            sprintf(sizebuf, "%4lluK",
                    ((unsigned long long) content->size) / 1024ULL);
        }
        else if (content->size < (10 * 1024 * 1024)) {
            float f = content->size;
            f /= (1024 * 1024);
            sprintf(sizebuf, "%1.2fM", f);
        }
        else if (content->size < (1024 * 1024 * 1024)) {
            sprintf(sizebuf, "%4lluM",
                    ((unsigned long long) content->size) /
                    (1024ULL * 1024ULL));
        }
        else {
            float f = (content->size / 1024);
            f /= (1024 * 1024);
            sprintf(sizebuf, "%1.2fG", f);
        }
        savedContents.append(*content);
        savedKeys.append(content->key);
    }

    data->keyCount += contentsCount;

    for (i = 0; i < commonPrefixesCount; i++) {
        savedCommonPrefixes.append(commonPrefixes[i]);
    }

    return S3StatusOK;
}

void list_bucket(const char *bucketName, const char *prefix,
                        const char *marker, const char *delimiter,
                        int maxkeys, const char* ak, const char* sk)
{

    S3_init();
    savedCommonPrefixes.clear();
    savedContents.clear();
    savedKeys.clear();

    S3BucketContext bucketContext =
    {
        0,
        bucketName,
        S3ProtocolHTTPS,
        S3UriStyleVirtualHost,
        ak,
        sk
    };

    S3ListBucketHandler listBucketHandler =
    {
        { &responsePropertiesCallback, &responseCompleteCallback },
        &listBucketCallback
    };

    list_bucket_callback_data data;

    snprintf(data.nextMarker, sizeof(data.nextMarker), "%s", marker);
    data.keyCount = 0;
    data.allDetails = 0;

    do {
        data.isTruncated = 0;
        do {
            S3_list_bucket(&bucketContext, prefix, data.nextMarker,
                           delimiter, maxkeys, 0, &listBucketHandler, &data);
        } while (S3_status_is_retryable((S3Status)statusG) && should_retry());
        if (statusG != S3StatusOK) {
            break;
        }
    } while (data.isTruncated && (!maxkeys || (data.keyCount < maxkeys)));

    if (statusG == S3StatusOK) {
//        if (!data.keyCount) {
//            printListBucketHeader(allDetails);
//        }
    }
    else {
        printError();
    }

    S3_deinitialize();
}

QString
getDate(int i) {
    if (i < savedCommonPrefixes.size()) return "";
    i -= savedCommonPrefixes.size();
    const S3ListBucketContent *content = &savedContents.at(i);

    char timebuf[256];
    time_t t = (time_t) content->lastModified;
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%dT%H:%M:%SZ",
             gmtime(&t));
    return QString(timebuf);
}

QString
getSize(int i) {
    if (i < savedCommonPrefixes.size()) return "";
    i -= savedCommonPrefixes.size();
    const S3ListBucketContent *content = &savedContents.at(i);

    char sizebuf[16];
    if (content->size < 100000) {
        sprintf(sizebuf, "%5llu", (unsigned long long) content->size);
    }
    else if (content->size < (1024 * 1024)) {
        sprintf(sizebuf, "%4lluK",
                ((unsigned long long) content->size) / 1024ULL);
    }
    else if (content->size < (10 * 1024 * 1024)) {
        float f = content->size;
        f /= (1024 * 1024);
        sprintf(sizebuf, "%1.2fM", f);
    }
    else if (content->size < (1024 * 1024 * 1024)) {
        sprintf(sizebuf, "%4lluM",
                ((unsigned long long) content->size) /
                (1024ULL * 1024ULL));
    }
    else {
        float f = (content->size / 1024);
        f /= (1024 * 1024);
        sprintf(sizebuf, "%1.2fG", f);
    }
    return QString(sizebuf);
    printf("%-50s  %s  %s\n", __func__, content->key,  sizebuf);
}

QString
getName(int i) {
    if (i >= savedCommonPrefixes.size()) {
        i -= savedCommonPrefixes.size();
        return savedKeys.at(i);
    } else {
        //const S3ListBucketContent *content = &;
        return QString(savedCommonPrefixes.at(i));
    }
}

int
getCount() {
    return savedCommonPrefixes.size() + savedContents.size();
}
