#include "DownloadManager.h"
#include "Helper/VMD5.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>
std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
#endif

using namespace std;
using namespace std::placeholders;

USING_NS_CC;
using namespace network;

#define DOWNLOAD_CACHE_FOLDER "DownloadFolder"
#define DOWNLOAD_CACHE_PATH "DownloadFolder/"

static DownloadManager* _instance = nullptr;

DownloadManager::DownloadManager()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    string path = cocos2d::FileUtils::getInstance()->getWritablePath() + string("/") + DOWNLOAD_CACHE_FOLDER;
    int ret = CreateDirectory(s2ws(path).c_str(), NULL);
#else
    string path = cocos2d::FileUtils::getInstance()->getWritablePath() + string("/") + DOWNLOAD_CACHE_FOLDER;
    FileUtils::getInstance()->createDirectory(path);
#endif
}

DownloadManager::~DownloadManager()
{
}

DownloadManager* DownloadManager::getInstance()
{
    if (_instance == nullptr)
        _instance = new DownloadManager();
    return _instance;
}

void DownloadManager::destroyInstance()
{
    if (_instance != nullptr)
        CC_SAFE_DELETE(_instance);
}

void DownloadManager::downloadImage(Sprite* sprite,
                                    const std::string& url,
                                    const std::string& defaulFrameId,
                                    cocos2d::Size size,
                                    const ImageResponseCallback& callback/* = nullptr*/,
                                    void* target /*= nullptr*/)
{
    //set default image while loading
    if (sprite != nullptr && defaulFrameId != "")
    {
        SpriteFrame* defaultFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(defaulFrameId);
        if (defaultFrame != nullptr)
            sprite->setSpriteFrame(defaultFrame);
    }
    DownloadImageHandler* downloadImageHandler = new DownloadImageHandler();
    this->_listRequestHandler.pushBack(downloadImageHandler);
    downloadImageHandler->setImageResponseCallback(callback);
    downloadImageHandler->setTarget(target);
    downloadImageHandler->downloadImage(sprite, url, size);
    downloadImageHandler->release();
}

void DownloadManager::requestData(const std::string& url, const DataResponseCallback& callback)
{
    RequestDataHandler* reqDataHandler = new RequestDataHandler();
    reqDataHandler->requestData(url, callback);
    this->_listRequestHandler.pushBack(reqDataHandler);
    reqDataHandler->release();
}

void DownloadManager::removeHandler(RequestHandler* handler)
{
    for (auto iter = this->_listRequestHandler.begin(); iter != this->_listRequestHandler.end();)
    {
        if ((*iter) == handler)
            iter = this->_listRequestHandler.erase(iter);
        else
            ++iter;
    }
}

void DownloadManager::removeHandlerForTarget(void* target)
{
    for (auto iter = this->_listRequestHandler.begin(); iter != this->_listRequestHandler.end(); iter++)
        if ((*iter)->getTarget() == target)
            (*iter)->setTarget(nullptr);
}

void DownloadManager::requestImageUrl(const std::string& url, DownloadImageHandler* handler)
{
    bool     finded = false;
    for (int i      = 0; i < _listImageRequest.size(); i++)
    {
        if (_listImageRequest[i].first == url)
        {
            _listImageRequest[i].second.pushBack(handler);
            finded = true;
            break;
        }
    }

    if (finded == false)
    {
        string                                 hexa = md5(url);
        cocos2d::Vector<DownloadImageHandler*> listHander;
        listHander.pushBack(handler);
        _listImageRequest.push_back(std::make_pair(url, listHander));

        HttpRequest* request = new HttpRequest();
        request->setUrl(url.c_str());
        request->setRequestType(HttpRequest::Type::GET);
        request->setResponseCallback(CC_CALLBACK_2(DownloadManager::onImageDownloadCompleted, this));
        request->setTag(hexa.c_str());
        HttpClient::getInstance()->send(request);
        request->release();
    }
}

void DownloadManager::finishRequestImageUrl(const std::string& url, bool success)
{
    for (auto iter1 = _listImageRequest.begin(); iter1 != _listImageRequest.end();)
    {
        if (iter1->first == url)
        {
            for (auto iter2 = iter1->second.begin(); iter2 != iter1->second.end();)
            {
                DownloadImageHandler* observer = (*iter2);
                if (observer)
                {
                    observer->onDownloadCompleted(url, success);
                    iter2 = iter1->second.erase(iter2);
                }
                else
                {
                    ++iter2;
                }
            }

            if (iter1->second.size() == 0)
                iter1 = _listImageRequest.erase(iter1);
            else
                ++iter1;
            break;
        }
    }
}

void DownloadManager::onImageDownloadCompleted(HttpClient* pSender, HttpResponse* pResponse)
{
    if (!pResponse)
    {
        CCLOG("Download Image: No Response");
        finishRequestImageUrl("", false);
        return;
    }
    HttpRequest* request = pResponse->getHttpRequest();
    const char * key     = request->getTag();
    const char * url     = request->getUrl();
    if (!pResponse->isSucceed())
    {
        CCLOG("Response failed");
        CCLOG("error buffer: %s", pResponse->getErrorBuffer());
        CCLOG("Download Image : error in downloaded");
        finishRequestImageUrl(url, false);
        return;
    }
    vector<char>* buffer = pResponse->getResponseData();

    CCLOG("Download Image: %s", url);
    Image* img = new(std::nothrow) Image();
    if (!img || !img->initWithImageData(reinterpret_cast<unsigned char*>(buffer->data()), buffer->size()))
    {
        CCLOG("Init Image Fail");
        finishRequestImageUrl(url, false);
        return;
    }
    CCLOG("Init Image Success");
    saveImage(img, key, url);

    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(img, key);
    if (texture)
        finishRequestImageUrl(url, true);
    img->release();
}

void DownloadManager::saveImage(Image* image, const std::string& name, const std::string& url)
{
    string savePath = cocos2d::FileUtils::getInstance()->getWritablePath() + (DOWNLOAD_CACHE_PATH + name + ".png");
    if (!image->saveToFile(savePath, false))
        CCLOG("Fail to save image file: %s\nSave path: %s", url.c_str(), savePath.c_str());
    else
        CCLOG("Save image successfully: %s\nSave path: %s", url.c_str(), savePath.c_str());
}


////////////////////////////////////////////////////////
////			   DOWNLOAD HANDLER					////
////////////////////////////////////////////////////////
RequestHandler::RequestHandler()
{
    _target = nullptr;
}

RequestHandler::~RequestHandler()
{
}

void RequestHandler::finish()
{
    DownloadManager::getInstance()->removeHandler(this);
}

void RequestHandler::setTarget(void* target)
{
    _target = target;
}

void* RequestHandler::getTarget()
{
    return _target;
}

DownloadImageHandler::DownloadImageHandler()
{
    this->_sprite   = nullptr;
    this->_url      = "";
    this->_callback = nullptr;
}

DownloadImageHandler::~DownloadImageHandler()
{
    CC_SAFE_RELEASE_NULL(_sprite);
}

void DownloadImageHandler::downloadImage(cocos2d::Sprite* sprite, const std::string& url1, Size size)
{
    if (sprite != nullptr)
    {
        this->_sprite = sprite;
        this->_sprite->retain();
    }
    this->_size = size;
    std::string url = url1;

    int pos;
    while ((pos = url.find(" ")) != std::string::npos)
    {
        url.replace(pos, 1, "%20");
    }
    string hexa = md5(url);
    Texture2D* texture = Director::getInstance()->getTextureCache()->getTextureForKey(hexa);

    //check image in ram
    if (texture == nullptr)
    {
        //check in download folder
        Image* image = loadImageFromCache(hexa);
        if (image == nullptr)
        {
            DownloadManager::getInstance()->requestImageUrl(url, this);
        }
        else
        {
            texture = Director::getInstance()->getTextureCache()->addImage(image, hexa);
            if (this->_sprite != nullptr)
            {
                setSpriteTexture(this->_sprite, texture);
            }
            else
            {
                this->_sprite = Sprite::createWithTexture(texture);
                this->_sprite->retain();
            }
            CCLOG("Download Image: load image from file\nurl: %s", url.c_str());
            image->release();
            finish(true);
        }
    }
    else
    {
        if (sprite != nullptr)
        {
            setSpriteTexture(this->_sprite, texture);
        }
        else
        {
            this->_sprite = Sprite::createWithTexture(texture);
            this->_sprite->retain();
        }
        CCLOG("Download Image: load image from cache\nurl: %s", url.c_str());
        finish(true);
    }
}

void DownloadImageHandler::setImageResponseCallback(const ImageResponseCallback& callback)
{
    _callback = callback;
}

void DownloadImageHandler::onDownloadCompleted(const std::string& url, bool success)
{
    if (!success)
    {
        finish(false);
        return;
    }
    string hexa = md5(url);
    Texture2D* texture = Director::getInstance()->getTextureCache()->getTextureForKey(hexa);
    //check image in ram
    if (texture != nullptr)
    {
        if (this->_sprite != nullptr)
        {
            setSpriteTexture(this->_sprite, texture);
        }
        else
        {
            this->_sprite = Sprite::createWithTexture(texture);
            this->_sprite->retain();
        }
    }
    else
    {
        finish(false);
    }
    finish(success);
}

void DownloadImageHandler::onImageDownloadCompleted(HttpClient* pSender, HttpResponse* pResponse)
{
    if (!pResponse)
    {
        CCLOG("Download Image: No Response");
        finish(false);
        return;
    }
    HttpRequest* request = pResponse->getHttpRequest();
    const char * key     = request->getTag();
    const char * url     = request->getUrl();
    if (!pResponse->isSucceed())
    {
        CCLOG("Response failed");
        CCLOG("error buffer: %s", pResponse->getErrorBuffer());
        CCLOG("Download Image : error in downloaded");
        finish(false);
        return;
    }
    vector<char>* buffer = pResponse->getResponseData();
    CCLOG("Download Image: %s", url);
    Image* img = new(std::nothrow) Image();
    if (!img || !img->initWithImageData(reinterpret_cast<unsigned char*>(buffer->data()), buffer->size()))
    {
        CCLOG("Init Image Fail");
        finish(false);
        return;
    }
    CCLOG("Init Image Success");
    saveImage(img, key, url);
    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(img, key);
    if (this->_sprite != nullptr)
    {
        setSpriteTexture(this->_sprite, texture);
    }
    else
    {
        this->_sprite = Sprite::createWithTexture(texture);
        this->_sprite->retain();
    }
    img->release();
    finish(true);
}

void DownloadImageHandler::finish(bool success)
{
    setSpriteSize(this->_size);
    if (_callback != nullptr && _target != nullptr)
        _callback(_sprite, success);
    RequestHandler::finish();
}

void DownloadImageHandler::saveImage(Image* image, const std::string& name, const std::string& url)
{
    string savePath = cocos2d::FileUtils::getInstance()->getWritablePath() + (DOWNLOAD_CACHE_PATH + name + ".png");
    if (!image->saveToFile(savePath, false))
        CCLOG("Fail to save image file: %s\nSave path: %s", url.c_str(), savePath.c_str());
    else
        CCLOG("Save image successfully: %s\nSave path: %s", url.c_str(), savePath.c_str());
}

Image* DownloadImageHandler::loadImageFromCache(const std::string& name)
{
    string path = cocos2d::FileUtils::getInstance()->getWritablePath() + (DOWNLOAD_CACHE_PATH + name + ".png");
    if (cocos2d::FileUtils::getInstance()->isFileExist(path))
    {
        Image* image = new Image();
        if (image->initWithImageFile(path))
            return image;
    }
    return nullptr;
}

void DownloadImageHandler::setSpriteSize(cocos2d::Size size)
{
    if (this->_sprite != nullptr && this->_sprite->getContentSize().width != 0 && this->_sprite->getContentSize().height != 0)
    {
        float scaleX = size.width / this->_sprite->getContentSize().width;
        float scaleY = size.height / this->_sprite->getContentSize().height;
        this->_sprite->setScaleX(scaleX);
        this->_sprite->setScaleY(scaleY);
    }
}

void DownloadImageHandler::setSpriteTexture(cocos2d::Sprite* sprite, cocos2d::Texture2D* texture)
{
    if (sprite != nullptr && texture != nullptr)
    {
        Rect rect = Rect::ZERO;
        rect.size = texture->getContentSize();
        SpriteFrame* spriteFrame = SpriteFrame::createWithTexture(texture, rect);
        sprite->setFlippedY(false);
        sprite->setSpriteFrame(spriteFrame);
    }
}

//// RequestDataHandler
RequestDataHandler::RequestDataHandler()
{
    _callback = nullptr;
    _data     = "";
}

RequestDataHandler::~RequestDataHandler()
{
}

void RequestDataHandler::finish()
{
    if (_callback != nullptr && _target != nullptr)
        _callback(_data);
    RequestHandler::finish();
}

void RequestDataHandler::requestData(const std::string& url, const DataResponseCallback& callback)
{
    _url      = url;
    _callback = callback;
    HttpRequest* request = new HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(RequestDataHandler::onRequestDataCompleted, this));
    HttpClient::getInstance()->send(request);
    request->release();
}

void RequestDataHandler::onRequestDataCompleted(cocos2d::network::HttpClient* pSender, cocos2d::network::HttpResponse* pResponse)
{
    if (!pResponse)
    {
        CCLOG("Download Image: No Response");
        finish();
        return;
    }
    HttpRequest* request = pResponse->getHttpRequest();
    int statusCode = pResponse->getResponseCode();
    const char* key = request->getTag();
    const char* url = request->getUrl();
    if (!pResponse->isSucceed())
    {
        CCLOG("Response failed");
        CCLOG("error buffer: %s", pResponse->getErrorBuffer());
        CCLOG("Request Data : error in request data");
        finish();
        return;
    }
    vector<char>* buffer = pResponse->getResponseData();
    CCLOG("Request Data: %s", url);
    std::string data(buffer->begin(), buffer->end());
    _data = data;
    finish();
}