﻿using FluentAssertions;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Logging.Abstractions;
using Xbim.Ifc4.GeometryResource;
using Xbim.IO.Memory;
using Xunit;

namespace Xbim.Geometry.Engine.Interop.Tests
{

    public class LoadGeometryEngine
    {
        private readonly IXbimGeometryServicesFactory factory;

        public LoadGeometryEngine(IXbimGeometryServicesFactory factory)
        {
            this.factory = factory;
        }

        [Fact]
        public void SimpleLoad()
        {
            var mm = new MemoryModel(new Ifc2x3.EntityFactoryIfc2x3());
            var geometryEngineV5 = factory.CreateGeometryEngineV5(mm, new NullLoggerFactory());
            geometryEngineV5.Should().NotBeNull();
            var modelGeometryService = factory.CreateModelGeometryService(mm, new NullLoggerFactory());
            modelGeometryService.Should().NotBeNull();
        }

       

        [Fact]
        public void TestLogging()
        {           
            using (var m = new MemoryModel(new Ifc4.EntityFactoryIfc4()))
            {
                using var loggerFactory = LoggerFactory.Create(builder => builder.AddConsole());
                var ge = factory.CreateGeometryEngineV5(m, loggerFactory);
                using (var txn = m.BeginTransaction("new"))
                {
                    var pline = m.Instances.New<IfcPolyline>();
                    ge.CreateCurve(pline);
                    
                }

            }
        }

       
    }
}
