#include "libhttproute/router.h"
#include "libhttproute/route.h"
#include "libhttproute/route_matcher_impl.h"

#include "libhttproute-qhttp.h"

#include <QCoreApplication>
#include <QString>

#include "qhttpserver.hpp"
#include "qhttpserverrequest.hpp"
#include "qhttpserverresponse.hpp"

using namespace qhttp::server;


/****************** demo request handlers ************************************/

class HelloRequestHandler :
	public RoutedReentrantQHttpRequestHandler
{
public:
	virtual void
	handle(QHttpRequest* req, QHttpResponse* res, const HR_NS::RouteMatch& rm)
	{
		res->setStatusCode(qhttp::ESTATUS_OK);
		res->end(QString("Hello %1!").arg(QString::fromStdString(rm.vars.at("name"))).toUtf8());
	}
};

class FooRequestHandler :
	public RoutedReentrantQHttpRequestHandler
{
public:
	virtual void
	handle(QHttpRequest* req, QHttpResponse* res, const HR_NS::RouteMatch&)
	{
		res->setStatusCode(qhttp::ESTATUS_OK);
		res->end("FOO!\n");
	}
};

class BarRequestHandler :
	public RoutedReentrantQHttpRequestHandler
{
public:
	virtual void
	handle(QHttpRequest* req, QHttpResponse* res, const HR_NS::RouteMatch&)
	{
		res->setStatusCode(qhttp::ESTATUS_OK);
		res->end("BAR!\n");
	}
};

/****************** main *****************************************************/

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);

	// Setup routes

	HR_NS::Router router;

	HelloRequestHandler helloHandler;
	FooRequestHandler fooHandler;
	BarRequestHandler barHandler;

	router.newRoute()
	.byMatcher(std::make_shared<HR_NS::RouteMethodMatcher>("GET"))
	.byMatcher(std::make_shared<HR_NS::RoutePathPrefixMatcher>("/method"))
	.withObject(&fooHandler);

	router.newRoute()
	.byMatcher(std::make_shared<HR_NS::RouteHeaderMatcher>("x-api", "123"))
	.byMatcher(std::make_shared<HR_NS::RoutePathPrefixMatcher>("/header"))
	.withObject(&barHandler);

	router.newRoute()
	.byMatcher(std::make_shared<HR_NS::RouteHostMatcher>("localhost"))
	.byMatcher(std::make_shared<HR_NS::RoutePathPrefixMatcher>("/host"))
	.withObject(&fooHandler);

	router.newRoute()
	.byMatcher(std::make_shared<HR_NS::RoutePathMatcher>("/path/{name:[a-z]+}"))
	.withObject(&helloHandler);

	router.newRoute()
	.byMatcher(std::make_shared<HR_NS::RoutePathPrefixMatcher>("/pathprefix"))
	.withObject(&fooHandler);

	std::map<std::string, std::string> params;
	params.insert(std::make_pair("foo", "bar"));
	router.newRoute()
	.byMatcher(std::make_shared<HR_NS::RoutePathPrefixMatcher>("/query"))
	.byMatcher(std::make_shared<HR_NS::RouteQueryParamsMatcher>(params))
	.withObject(&barHandler);

	//router.newRoute()
	//.byMatcher(std::make_shared<HR_NS::RouteSchemeMatcher>("http"))
	//.byMatcher(std::make_shared<HR_NS::RoutePathPrefixMatcher>("/scheme"))
	//.withObject(&fooHandler);

	// Start server and handle requests

	QHttpServer server(&app);
	server.listen("1337", [&](QHttpRequest * req, QHttpResponse * res)
	{
		QHttpHttpServerRequestImpl r(req);
		const auto t = router.findFirstRoute(r);
		const auto& route = std::get<0>(t);
		if (route && route->object())
		{
			auto handler = static_cast<RoutedReentrantQHttpRequestHandler*>(route->object());
			handler->handle(req, res, std::get<1>(t));
		}
		else
		{
			res->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
			res->end();
		}
	});
	if (!server.isListening())
	{
		return -1;
	}
	return app.exec();
}