#pragma once

#include "types.hh"
#include "hash.hh"

#include <string>

namespace nix {

struct DownloadOptions
{
    std::string expectedETag;
    bool verifyTLS = false;
    enum { yes, no, automatic } showProgress = yes;
    bool head = false;
    size_t tries = 1;
};

struct DownloadResult
{
    bool cached;
    string etag;
    std::shared_ptr<std::string> data;
};

class Store;

struct Downloader
{
    virtual DownloadResult download(string url, const DownloadOptions & options) = 0;

    Path downloadCached(ref<Store> store, const string & url, bool unpack,
        const Hash & expectedHash = Hash());

    enum Error { NotFound, Forbidden, Misc, Transient };
};

ref<Downloader> makeDownloader();

class DownloadError : public Error
{
public:
    Downloader::Error error;
    DownloadError(Downloader::Error error, const FormatOrString & fs)
        : Error(fs), error(error)
    { }
};

bool isUri(const string & s);

}
