#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QStringList>
#include <QEventLoop>
#include "zlib/zlib.h"
#include "marketsession.h"


MarketSession::MarketSession(QObject *parent) :
    QObject(parent),
    SERVICE("android"),
    URL_LOGIN("https://www.google.com/accounts/ClientLogin")

{
    context.set_issecure(false);
    context.set_version(2009011);
    context.set_usercountry("GB");
    context.set_userlanguage("en");
    context.set_deviceandsdkversion("passion:9");
    context.set_operatoralpha("T-Mobile");
    context.set_simoperatoralpha("T-Mobile");
    context.set_operatornumeric("310260");
    context.set_simoperatornumeric("310260");
}

Response_ResponseGroup MarketSession::execute(Request_RequestGroup requestGroup)
{

    request.mutable_context()->CopyFrom(context);
    AppsRequest app;
    app.set_query("pname:com.kebab.Llama");
    app.set_startindex(0);
    app.set_entriescount(10);

    request.add_requestgroup()->CopyFrom(requestGroup);

    qDebug(request.DebugString().c_str());
    qDebug(executeProtobuf(request).DebugString().c_str());

}


void MarketSession::login(QString email, QString password, QString androidId,QString accountType)
{
    setAndroidId(androidId);
    QMap<QString,QString> params;
    params.insert("Email",email);
    params.insert("Passwd", password);
    params.insert("service", SERVICE);
    params.insert("accountType", accountType);
    postUrl(URL_LOGIN,params);

}
void MarketSession::loginFinished()
{
    qDebug("postURL");
    QString authKey;
    QStringList st=QString(http->readAll()).split(QRegExp("[\n\r=]"));
    while(!st.empty())
    {
        if(st.takeFirst()==QString("Auth"))
        {
            authKey=st.takeFirst();
            break;
        }
    }
    qDebug(authKey.toAscii());
    setAuthSubToken(authKey);
    emit logged();
    http->deleteLater();
}

void MarketSession::postUrl(const QString& url, QMap<QString, QString> params)
{
    QString data="";
    foreach(const QString& key,params.keys())
    {
        data.append(QString("&%1=%2").arg(key).arg(params.value(key)));
    }
    data.remove(0,1);
    qDebug(data.toAscii());
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    http = qnam.post(req,data.toUtf8());
    connect(http,SIGNAL(finished()),this, SLOT(loginFinished()));
}


Response MarketSession::executeProtobuf(Request request)
{
    QByteArray requestBytes(request.SerializeAsString().c_str(),request.ByteSize());
    QByteArray responseBytes;
    if(!context.issecure())
        responseBytes = executeRawHttpQuery(requestBytes);
    else
        responseBytes = executeRawHttpsQuery(requestBytes);
    Response r;
    r.ParseFromArray(responseBytes.constData(),responseBytes.size());

    return r;
}

QByteArray MarketSession::executeRawHttpQuery(const QByteArray &request)
{
    QByteArray data;
    QUrl url("http://android.clients.google.com/market/api/ApiRequest");
    QNetworkAccessManager manager;
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setRawHeader("Cookie",QString("ANDROID=%1").arg(authSubToken).toUtf8());
    req.setRawHeader("User-Agent", "Android-Market/2 (sapphire PLAT-RC33); gzip");
    req.setRawHeader("Accept-Charset","ISO-8859-1,utf-8;q=0.7,*;q=0.7");

    QString requestData=QString("version=%1&request=%2")
            .arg(PROTOCOL_VERSION).arg(QString(request.toBase64()));
    QNetworkReply* http = manager.post(req,requestData.toUtf8());
    QEventLoop eventLoop;
    connect(http,SIGNAL(finished()),&eventLoop, SLOT(quit()));
    eventLoop.exec();
    data=gzipDecompress(http->read(http->header(QNetworkRequest::ContentLengthHeader).toUInt()));
    delete http;
    return data;
}

QByteArray MarketSession::executeRawHttpsQuery(const QByteArray &request)
{
    QByteArray data;
    QUrl url("https://android.clients.google.com/market/api/ApiRequest");
    QNetworkAccessManager manager;
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setRawHeader("Cookie",QString("ANDROIDSECURE=%1").arg(authSubToken).toUtf8());
    req.setRawHeader("User-Agent", "Android-Market/2 (sapphire PLAT-RC33); gzip");
    req.setRawHeader("Accept-Charset","ISO-8859-1,utf-8;q=0.7,*;q=0.7");

    QString requestData=QString("version=%1&request=%2")
            .arg(PROTOCOL_VERSION).arg(QString(request.toBase64()));
    QNetworkReply* http = manager.post(req,requestData.toUtf8());
    QEventLoop eventLoop;
    connect(http,SIGNAL(finished()),&eventLoop, SLOT(quit()));
    eventLoop.exec();
    data=gzipDecompress(http->read(http->header(QNetworkRequest::ContentLengthHeader).toUInt()));
    delete http;
    return data;
}

QByteArray MarketSession::gzipDecompress(QByteArray compressData)
{
    //decompress GZIP data
    //strip header and trailer
      compressData.remove(0, 10);
      compressData.chop(12);

      const int buffersize = 16384;
      quint8 buffer[buffersize];

      z_stream cmpr_stream;
      cmpr_stream.next_in = (unsigned char *)compressData.data();
      cmpr_stream.avail_in = compressData.size();
      cmpr_stream.total_in = 0;

      cmpr_stream.next_out = buffer;
      cmpr_stream.avail_out = buffersize;
      cmpr_stream.total_out = 0;

      cmpr_stream.zalloc = Z_NULL;
      cmpr_stream.zalloc = Z_NULL;

      if( inflateInit2(&cmpr_stream, -8 ) != Z_OK) {
              qDebug("cmpr_stream error!");
      }

        QByteArray uncompressed;
        do {
                int status = inflate( &cmpr_stream, Z_SYNC_FLUSH );

                if(status == Z_OK || status == Z_STREAM_END) {
                        uncompressed.append(QByteArray::fromRawData(
                             (char *)buffer,
                             buffersize - cmpr_stream.avail_out));
                        cmpr_stream.next_out = buffer;
                        cmpr_stream.avail_out = buffersize;
                } else {
                         inflateEnd(&cmpr_stream);
                        }

                if(status == Z_STREAM_END) {
                    inflateEnd(&cmpr_stream);
                    break;
                }

        }while(cmpr_stream.avail_out == 0);

        return uncompressed;
}
