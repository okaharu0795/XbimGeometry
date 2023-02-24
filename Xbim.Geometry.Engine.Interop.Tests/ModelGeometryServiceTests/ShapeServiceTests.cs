﻿using FluentAssertions;
using Microsoft.Extensions.Logging;
using System;
using Xbim.Geometry.Abstractions;
using Xbim.Geometry.Engine.Interop;
using Xbim.Ifc4;
using Xbim.IO.Memory;
using Xunit;

namespace Xbim.Geometry.Engine.Tests
{

    public class ShapeServiceTests
    {
        #region Setup

        
        private readonly MemoryModel _dummyModel = new MemoryModel(new EntityFactoryIfc4());
        private readonly IXModelGeometryService _modelSvc;
        private readonly IXShapeService _shapeService;
        private readonly IXbimGeometryServicesFactory factory;


        #endregion


        public ShapeServiceTests(IXShapeService shapeService, IXbimGeometryServicesFactory factory,ILoggerFactory loggerFactory)
        {
            _shapeService = shapeService;
            this.factory = factory;
            _modelSvc = factory.CreateModelGeometryService(_dummyModel, loggerFactory);
        }

        [Fact]
        public void CanScaleShape()
        {
            var solidService = _modelSvc.SolidFactory;
            var ifcSweptDisk = IfcMoq.IfcSweptDiskSolidMoq(startParam: 0, endParam: 100, radius: 30, innerRadius: 15);
            var solid = (IXSolid)solidService.Build(ifcSweptDisk);
            var scale = 0.001;

            Assert.False(solid.IsEmptyShape());
            Assert.True(solid.IsValidShape());

            var moved = _shapeService.Scaled(solid, scale);

            moved.Should().NotBeNull();
            var volDiff = Math.Abs(moved.As<IXSolid>().Volume - solid.Volume * Math.Pow(scale, 3));
            volDiff.Should().BeLessThan(Math.Pow(scale, 3));
        }
    }

}

