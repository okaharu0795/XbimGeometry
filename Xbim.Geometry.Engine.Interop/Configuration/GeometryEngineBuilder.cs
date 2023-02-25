﻿using Microsoft.Extensions.DependencyInjection;


namespace Xbim.Geometry.Engine.Configuration
{
    internal class GeometryEngineBuilder : IGeometryEngineBuilder
    {
        public IServiceCollection Services { get; }

        public GeometryEngineBuilder(IServiceCollection services)
        {
            Services = services;
        }
    }
}
