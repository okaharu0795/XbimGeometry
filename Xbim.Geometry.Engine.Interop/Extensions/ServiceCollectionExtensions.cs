﻿using Microsoft.Extensions.DependencyInjection;
using System;
using System.IO;
using System.Reflection;
using Xbim.Geometry.Engine.Interop.Internal;
using Xbim.Ifc4.Interfaces;

namespace Xbim.Geometry.Engine.Interop
{
    public static class ServiceCollectionExtensions
    {
        /// <summary>
        /// Adds xbim geometry to the specified <see cref="IServiceCollection"/>
        /// </summary>
        /// <param name="services"></param>
        /// <returns>The <see cref="IServiceCollection"/> so additional calls can be chained</returns>
        public static IServiceCollection AddGeometryServices(this IServiceCollection services)
        {
            

            services.AddScoped<IXbimGeometryEngine, XbimGeometryEngine>();
            services.AddSingleton<IXbimGeometryServicesFactory, XbimGeometryServicesFactory>();
            services.AddXbimGeometryServices();
            return services;
        }

        private static IServiceCollection AddXbimGeometryServices(this IServiceCollection services)
        {
            // We can't invoke the mixed mode/native code directly. We have to go via reflection to invoke:
            // Xbim.Geometry.DependencyInjection.ServiceCollectionExtensions.AddXbimGeometryEngine(services);

            Assembly geometryAssembly = ManagedGeometryAssembly;
            var serviceExtensionsType = geometryAssembly.GetType(XbimArchitectureConventions.ServiceCollectionExtensionsName);

            var serviceCollectionExtensions = Activator.CreateInstance(serviceExtensionsType);
            var method = serviceExtensionsType.GetMethod(XbimArchitectureConventions.AddGeometryEngineServicesName);
            method.Invoke(serviceCollectionExtensions, new object[] { services });

            return services;
        }

        private static Lazy<Assembly> lazyAssembly = new Lazy<Assembly>(() => 
        {
            var codeDir = AppDomain.CurrentDomain.BaseDirectory;
            return LoadDll(codeDir);
        });

        private static Assembly ManagedGeometryAssembly { get => lazyAssembly.Value; } 
        
           
        private static Assembly LoadDll(string currentDir)
        {
            var archFolder = XbimArchitectureConventions.Runtime;
            var dllPath = Path.Combine(currentDir, archFolder, XbimArchitectureConventions.ModuleDllName);

            Assembly geomAssembly;
            if (File.Exists(dllPath))
            {
                geomAssembly = Assembly.LoadFile(dllPath);
            }
            else
            {
                // Functions puts bins in a sub-folder under the runtime. e.g. bin/Debug/net48/bin  - while we auto copy GE bins to the typical parent folder
                var parentDir = Path.GetDirectoryName(currentDir);
                var parentDllPath = Path.Combine(parentDir, archFolder, XbimArchitectureConventions.ModuleDllName);
                if (File.Exists(parentDllPath))
                {
                    geomAssembly = Assembly.LoadFile(parentDllPath);
                }
                else
                {
                    throw new Exception($"Unable to locate v6 Geometry Engine in {archFolder}. \n\nLooked in {dllPath} &\n {parentDllPath}");
                }
            }

            return geomAssembly;
        }
    }
}
