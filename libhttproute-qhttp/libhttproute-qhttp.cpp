#include "libhttproute-qhttp.h"

#include <QString>
#include <QUrl>
#include <QUrlQuery>

QHttpHttpServerRequestImpl::QHttpHttpServerRequestImpl(qhttp::server::QHttpRequest* r) :
	HR_NS::HttpServerRequest(),
	_r(r)
{}

QHttpHttpServerRequestImpl::~QHttpHttpServerRequestImpl()
{
	_r = nullptr;
}

std::string
QHttpHttpServerRequestImpl::getMethod() const
{
	auto method = _r->methodString();
	method = method.mid(0, -1);
	return std::move(method.toStdString());
}

std::string
QHttpHttpServerRequestImpl::getPath() const
{
	const auto& url = _r->url();
	auto path = url.path();
	return std::move(path.toStdString());
}

std::string
QHttpHttpServerRequestImpl::getHost() const
{
	const auto& map = _r->headers();
	auto itr = map.find("host");
	if (itr != map.end())
	{
		return std::move(itr.value().toStdString());
	}
	return std::string();
}

std::string
QHttpHttpServerRequestImpl::getScheme() const
{
	const auto& url = _r->url();
	return std::move(url.scheme().toStdString());
}

bool
QHttpHttpServerRequestImpl::hasHeader(const std::string& key) const
{
	auto k = QByteArray::fromStdString(key);
	return _r->headers().contains(k);
}

std::string
QHttpHttpServerRequestImpl::getHeader(const std::string& key, const std::string& defaultValue)
{
	auto k = QByteArray::fromStdString(key);
	const auto& map = _r->headers();
	auto itr = map.find(k);
	if (itr != map.end())
	{
		return std::move(itr.value().toStdString());
	}
	return defaultValue;
}

std::vector<std::string>
QHttpHttpServerRequestImpl::getHeaders(const std::string& key) const
{
	std::vector<std::string> vec;
	auto k = QByteArray::fromStdString(key);
	const auto& map = _r->headers();
	auto itr = map.find(k);
	for (; itr != map.end(); ++itr)
	{
		vec.push_back(std::move(itr.value().toStdString()));
	}
	return vec;
}

bool
QHttpHttpServerRequestImpl::hasParameter(const std::string& key) const
{
	auto k = QString::fromStdString(key);
	QUrlQuery q(_r->url());
	return q.hasQueryItem(k);
}

std::string
QHttpHttpServerRequestImpl::getParameter(const std::string& name, const std::string& defaultValue) const
{
	auto k = QString::fromStdString(name);
	QUrlQuery q(_r->url());
	if (q.hasQueryItem(k))
	{
		return std::move(q.queryItemValue(k).toStdString());
	}
	return defaultValue;
}

std::vector<std::string>
QHttpHttpServerRequestImpl::getParameters(const std::string& name) const
{
	std::vector<std::string> vec;
	auto k = QString::fromStdString(name);
	QUrlQuery q(_r->url());
	for (const auto& v : q.allQueryItemValues(k))
	{
		vec.push_back(std::move(v.toStdString()));
	}
	return std::vector<std::string>();
}