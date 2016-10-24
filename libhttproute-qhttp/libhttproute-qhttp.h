#pragma once

#include "libhttproute/request.h"

#include <QObject>

#include "qhttpserverrequest.hpp"

/*
*/
class QHttpHttpServerRequestImpl :
	public HR_NS::HttpServerRequest
{
public:
	QHttpHttpServerRequestImpl(qhttp::server::QHttpRequest* r);
	virtual ~QHttpHttpServerRequestImpl();

	virtual std::string getMethod() const;
	virtual std::string getPath() const;
	virtual std::string getHost() const;
	virtual std::string getScheme() const;

	virtual bool hasHeader(const std::string& key) const;
	virtual std::string getHeader(const std::string& key, const std::string& defaultValue = std::string());
	virtual std::vector<std::string> getHeaders(const std::string& key) const;

	virtual bool hasParameter(const std::string& key) const;
	virtual std::string getParameter(const std::string& name, const std::string& defaultValue = std::string()) const;
	virtual std::vector<std::string> getParameters(const std::string& name) const;

private:
	qhttp::server::QHttpRequest* _r;
};


/*
*/
class RoutedReentrantQHttpRequestHandler
{
public:
	virtual void
	handle(qhttp::server::QHttpRequest* req, qhttp::server::QHttpResponse* res, const HR_NS::RouteMatch& rm) = 0;
};