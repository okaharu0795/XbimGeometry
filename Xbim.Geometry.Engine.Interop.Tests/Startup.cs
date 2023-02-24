﻿using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using System;
using Xbim.Common.Configuration;
using Xbim.Geometry.Abstractions;
using Xbim.Geometry.Engine.Interop;
using Xbim.Geometry.Engine.Interop.Extensions;
using Xunit.DependencyInjection;
using Xunit.DependencyInjection.Logging;
namespace Xbim.Geometry.Engine.Tests
{
    public class Startup
    {
#if DEBUG

        const LogLevel DefaultLogLevel = LogLevel.Debug;
#else
        const LogLevel DefaultLogLevel = LogLevel.Information;
#endif
        public void Configure(ILoggerFactory loggerFactory, ITestOutputHelperAccessor accessor, IServiceProvider serviceProvider)
        {

            loggerFactory.AddProvider(
                new XunitTestOutputLoggerProvider(accessor, (name, level) => level is >= DefaultLogLevel and < LogLevel.None)
                );

        }

        public void ConfigureServices(IServiceCollection services)
        {
            services.AddLogging(configure => configure.SetMinimumLevel(DefaultLogLevel))
                .AddXbimToolkit() // Required if we need IfcStore ModelProvider functionality
                .AddLogging()
                .AddGeometryServices(opt => opt.Configure(o => o.GeometryEngineVersion = XGeometryEngineVersion.V5));


            // Re-use this Service Collection in the internal xbim DI
            // We can't substitute Xunit.DependencyInjection's IServiceProvider directly since it's a scoped provider which has a per test lifetime
            // and we need the root ServiceProvider. This means we have two ServiceProvider instances in the tests.

            XbimServices.Current.UseExternalServiceCollection(services);



        }
    }
}
