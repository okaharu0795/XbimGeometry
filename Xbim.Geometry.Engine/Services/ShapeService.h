#pragma once
#include "../XbimGeometryObject.h"
using namespace Xbim::Geometry;
using namespace Xbim::Geometry::Abstractions;
namespace Xbim
{
	namespace Geometry
	{

		namespace Services
		{

			public ref class ShapeService : IXShapeService
			{
              

            public:
                virtual IXShape^ Convert(System::String^ brepString);

                virtual System::String^ Convert(IXShape^ shape);

                virtual System::String^ Convert(IXbimGeometryObject^ v5Shape);

                virtual IXbimGeometryObject^ ConvertToV5(System::String^ brepString);

                virtual IXShape^ Cut(IXShape^ body, IXShape^ subtraction);


                virtual IXShape^ Cut(IXShape^ body, IEnumerable<IXShape^>^ subtractions);

                virtual IXShape^ Transform(IXShape^ shape, XbimMatrix3D transformMatrix);

                /// <summary>
                /// Adds triangulation data to the shape, mostly for internal use
                /// </summary>
                /// <param name="shape"></param>
                virtual void Triangulate(IXShape^ shape);

                virtual IXShape^ RemovePlacement(IXShape^ shape);

                virtual IXShape^ SetPlacement(IXShape^ shape, IIfcObjectPlacement^ placement) ;

                virtual IXShape^ UnifyDomain(IXShape^ shape);

                virtual IXShape^ Union(IXShape^ body, IXShape^ addition);

                virtual IXShape^ Union(IXShape^ body, IEnumerable<IXShape^>^ additions);

                virtual IXShape^ Moved(IXShape^ shape, IIfcObjectPlacement^ placement, bool invertPlacement);

                virtual IXShape^ Transform(IXShape^ shape, IXMatrix^ transform);

                virtual IXShape^ Intersect(IXShape^ shape, IXShape^ intersect );
                virtual IXShape^ Intersect(IXShape^ shape, IEnumerable<IXShape^>^ intersect);
                virtual IXShape^ Moved(IXShape^ shape, IXLocation^ location);

                virtual IXShape^ Scaled(IXShape^ shape, double scale);
                virtual IXShape^ Combine(IXShape^ shape, IEnumerable<IXShape^>^ intersect);

                virtual bool IsFacingAwayFrom(IXFace^ face, IXDirection^ direction);

                virtual IXbimGeometryObject^ ConvertToV5(IXShape^ shape);

                virtual IXShape^ Combine(IEnumerable<IXShape^>^ shapes);

                virtual array<System::Byte>^ CreateWexBimMesh(IXShape^ shape, IXMeshFactors^ meshFactors, double scale, IXAxisAlignedBoundingBox^% bounds,  bool% hasCurves);
               
                virtual array<System::Byte>^ CreateWexBimMesh(IXShape^ shape, IXMeshFactors^ meshFactors, double scale, IXAxisAlignedBoundingBox^% bounds);
                static array<System::Byte>^ CreateWexBimMesh(IXShape^ shape, IXMeshFactors^ meshFactors, double scale, IXAxisAlignedBoundingBox^% bounds, bool% hasCurvesbool, bool checkEdges, bool cleanBefore, bool cleanAfter);
            };
		}
	}
}
