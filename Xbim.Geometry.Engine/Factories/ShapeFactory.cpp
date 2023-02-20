#include "ShapeFactory.h"
#include "GeometryFactory.h"
#include "ShellFactory.h"
#include <TopTools_ListOfShape.hxx>
#include <TopoDS.hxx>
#include <HLRBRep_PolyAlgo.hxx>
#include <HLRBRep_PolyHLRToShape.hxx>
#include <TopExp.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <gp_Trsf.hxx>

#include <BRep_Builder.hxx>
#include "../BRep/XShape.h"
#include "../BRep/XVertex.h"
#include "../BRep/XEdge.h"
#include "../BRep/XWire.h"
#include "../BRep/XFace.h"
#include "../BRep/XShell.h"
#include "../BRep/XSolid.h"
#include "../BRep/XCompound.h"
#include "../Brep/XPlane.h"
#include "../Brep/XLocation.h"


#include "../XbimConvert.h"
#include "../XbimVertex.h"
#include "../XbimEdge.h"
#include "../XbimWire.h"
#include "../XbimFace.h"
#include "../XbimShell.h"
#include "../XbimSolid.h"
#include "../XbimCompound.h"
#include "../BRep/OccExtensions/LineSegment2d.h"
#include "../Factories/Unmanaged/NBooleanFactory.h"

#include "../BRep/XPolyLoop2d.h"
#include "../XbimGeometryObject.h"
#include <ShapeFix_Solid.hxx>


using namespace Xbim::Geometry::BRep;
namespace Xbim
{
	namespace Geometry
	{
		namespace Factories
		{
			IXShape^ ShapeFactory::Convert(System::String^ brepStr)
			{
				const char* cStr = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(brepStr)).ToPointer();
				try
				{
					TopoDS_Shape shape = EXEC_NATIVE->Convert(cStr);
					switch (shape.ShapeType())
					{
					case TopAbs_VERTEX:
						return gcnew XVertex(TopoDS::Vertex(shape));
					case TopAbs_EDGE:
						return gcnew XEdge(TopoDS::Edge(shape));
					case TopAbs_WIRE:
						return gcnew XWire(TopoDS::Wire(shape));
					case TopAbs_FACE:
						return gcnew XFace(TopoDS::Face(shape));
					case TopAbs_SHELL:
						return gcnew XShell(TopoDS::Shell(shape));
					case TopAbs_SOLID:
						return gcnew XSolid(TopoDS::Solid(shape));
					case TopAbs_COMPOUND:
						return  gcnew XCompound(TopoDS::Compound(shape));
					case TopAbs_COMPSOLID:
					default:
						LogError("Unsupported Shape Type, Compound Solid");
					}
				}
				catch (...)
				{
					throw gcnew XbimGeometryServiceException("Failure to convert from Brep string");
				}
				finally
				{
				System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)cStr));
				}
				throw gcnew XbimGeometryServiceException("Failure to convert from Brep string");
			}


			/*IXbimGeometryObject^ ShapeFactory::ConvertToV5(System::String^ brepStr)
			{

				return _modelService->GetV5GeometryEngine()->FromBrep(brepStr);
			}
			IXbimGeometryObject^ ShapeFactory::ConvertToV5(IXShape^ shape)
			{
				try
				{
					const TopoDS_Shape& topoShape = TOPO_SHAPE(shape);
					switch (topoShape.ShapeType())
					{
					case TopAbs_VERTEX:
						return gcnew XbimVertex(TopoDS::Vertex(topoShape));
					case TopAbs_EDGE:
						return gcnew XbimEdge(TopoDS::Edge(topoShape));
					case TopAbs_WIRE:
						return gcnew XbimWire(TopoDS::Wire(topoShape));
					case TopAbs_FACE:
						return gcnew XbimFace(TopoDS::Face(topoShape));
					case TopAbs_SHELL:
						return gcnew XbimShell(TopoDS::Shell(topoShape));
					case TopAbs_SOLID:
						return gcnew XbimSolid(TopoDS::Solid(topoShape));
					case TopAbs_COMPOUND:
						return  gcnew XbimCompound(TopoDS::Compound(topoShape), true, _modelService->Precision);
					case TopAbs_COMPSOLID:
					default:
						LogError("Unsupported Shape Type, Compound Solid");
					}
				}
				catch (...)
				{
					throw gcnew XbimGeometryServiceException("Failure to convert from IXShape");
				}

				throw gcnew XbimGeometryServiceException("Failure to convert from from IXShape");

			}*/
			System::String^ ShapeFactory::Convert(IXShape^ shape)
			{
				return shape->BrepString();
			}
			System::String^ ShapeFactory::Convert(IXbimGeometryObject^ shape)
			{
				//need to make native
				XbimGeometryObject^ geomObj = dynamic_cast<XbimGeometryObject^>(shape);
				if (geomObj != nullptr)
					return geomObj->ToBRep;
				else
					return System::String::Empty;
			}
			/// <summary>
			/// Need to finish for all shapes
			/// </summary>
			/// <param name="toFix"></param>
			/// <returns></returns>
			IXShape^ ShapeFactory::UnifyDomain(IXShape^ shape)
			{
				const TopoDS_Shape& topoShape = TOPO_SHAPE(shape);
				TopoDS_Shape r = NUnifyDomain(topoShape);
				return GetXbimShape(r);
			}

			TopoDS_Shape ShapeFactory::NUnifyDomain(const TopoDS_Shape& toFix)
			{
				return OccHandle().UnifyDomain(toFix, _modelService->Precision, Precision::Angular());
			}

			IXShape^ ShapeFactory::Build(IIfcGeometricRepresentationItem^ geomRep)
			{
				//IXbimGeometryObject^ brepV5 = _modelService->GetV5GeometryEngine()->Create(geomRep, Logger());
				//if (brepV5 != nullptr && brepV5->IsValid)
				//{
				//	XbimSetObject^ geomSet = dynamic_cast<XbimSetObject^>(brepV5); //do we have a set
				//	XbimOccShape^ geomShape = dynamic_cast<XbimOccShape^>(brepV5); //or a simple shape
				//	if (geomShape != nullptr)
				//		return GetXbimShape((const TopoDS_Shape&)geomShape);
				//	else if (geomSet != nullptr)
				//		return GetXbimShape((const TopoDS_Shape&)geomSet);
				//	else
				//		return nullptr;
				//}
				//else
				return nullptr;
			}

			IXShape^ ShapeFactory::GetXbimShape(const TopoDS_Shape& shape)
			{
				return XShape::GetXbimShape(shape);
			}

			IXShape^ ShapeFactory::Transform(IXShape^ shape, XbimMatrix3D matrix)
			{

				XShapeType shapeType = shape->ShapeType;
				switch (shapeType)
				{
				case XShapeType::Vertex:
					return gcnew XVertex(TopoDS::Vertex(Transform(*static_cast<XVertex^>(shape)->Ptr(), matrix)));
				case XShapeType::Edge:
					return gcnew XEdge(TopoDS::Edge(Transform(*static_cast<XEdge^>(shape)->Ptr(), matrix)));
				case XShapeType::Wire:
					return gcnew XWire(TopoDS::Wire(Transform(*static_cast<XWire^>(shape)->Ptr(), matrix)));
				case XShapeType::Face:
					return gcnew XFace(TopoDS::Face(Transform(*static_cast<XFace^>(shape)->Ptr(), matrix)));
				case XShapeType::Shell:
					return gcnew XShell(TopoDS::Shell(Transform(*static_cast<XShell^>(shape)->Ptr(), matrix)));
				case XShapeType::Solid:
					return gcnew XSolid(TopoDS::Solid(Transform(*static_cast<XSolid^>(shape)->Ptr(), matrix)));
				case XShapeType::Compound:
					return gcnew XCompound(TopoDS::Compound(Transform(*static_cast<XCompound^>(shape)->Ptr(), matrix)));
				default:
					break;
				}
				throw gcnew XbimGeometryServiceException("Transform not supported for this shape type: " + shapeType.ToString());
			}

			IXShape^ ShapeFactory::Union(IXShape^ body, IXShape^ addition)
			{
				const TopoDS_Shape& topoBody = TOPO_SHAPE(body);
				const TopoDS_Shape& topoAddition = TOPO_SHAPE(addition);
				TopoDS_Shape result = OccHandle().Union(topoBody, topoAddition, _modelService->MinimumGap);
				return GetXbimShape(result);
			}
			

			IXShape^ ShapeFactory::Cut(IXShape^ body, IXShape^ substraction)
			{
				const TopoDS_Shape& topoBody = TOPO_SHAPE(body);
				const TopoDS_Shape& topoSubstraction = TOPO_SHAPE(substraction);
				TopoDS_Shape result = OccHandle().Cut(topoBody, topoSubstraction, _modelService->MinimumGap);
				return GetXbimShape(result);
			}

			IXShape^ ShapeFactory::Union(IXShape^ body, System::Collections::Generic::IEnumerable<IXShape^>^ additionShapes)
			{
				const TopoDS_Shape& topoBody = TOPO_SHAPE(body);

				TopTools_ListOfShape additions;
				for each (IXShape ^ addition in additionShapes)
				{
					const TopoDS_Shape& topoAddition = TOPO_SHAPE(addition);
					additions.Append(topoAddition);
				}
				TopoDS_Shape result = OccHandle().Union(topoBody, additions, _modelService->MinimumGap);
				return GetXbimShape(result);
			}

			IXShape^ ShapeFactory::Cut(IXShape^ body, System::Collections::Generic::IEnumerable<IXShape^>^ substractionShapes)
			{
				const TopoDS_Shape& topoBody = TOPO_SHAPE(body);
				TopTools_ListOfShape substractions;
				for each (IXShape ^ substraction in substractionShapes)
				{
					const TopoDS_Shape& topoSubstraction = TOPO_SHAPE(substraction);
					substractions.Append(topoSubstraction);
				}
				TopoDS_Shape result = OccHandle().Cut(topoBody, substractions, _modelService->MinimumGap);

				return GetXbimShape(result);
			}

			IXShape^ ShapeFactory::RemovePlacement(IXShape^ shape)
			{
				TopoDS_Shape topoShape = TOPO_SHAPE(shape);
				topoShape.Location(TopLoc_Location()); //blank it off
				return  GetXbimShape(topoShape);
			}

			IXShape^ ShapeFactory::SetPlacement(IXShape^ shape, IIfcObjectPlacement^ placement)
			{
				TopoDS_Shape s = TOPO_SHAPE(shape);

				if (s.IsNull()) return nullptr;
				s.Location(XbimConvert::ToLocation(placement, Logger()));
				return  GetXbimShape(s);
			}


			IXShape^ ShapeFactory::Moved(IXShape^ shape, IIfcObjectPlacement^ placement, bool invertPlacement)
			{
				TopoDS_Shape s = TOPO_SHAPE(shape);
				if (invertPlacement)
					s.Move(XbimConvert::ToLocation(placement, Logger()).Inverted());
				else
					s.Move(XbimConvert::ToLocation(placement, Logger()));
				return  GetXbimShape(s);
			}

			IXShape^ ShapeFactory::Moved(IXShape^ shape, IXLocation^ moveTo)
			{
				TopoDS_Shape s = TOPO_SHAPE(shape);
				auto xLoc = static_cast<XLocation^>(moveTo);
				s.Move(xLoc->Ref());
				return  GetXbimShape(s);
			}

			IXFace^ ShapeFactory::Add(IXFace^ toFace, array<IXWire^>^ wires)
			{
				TopoDS_Face occFace = TOPO_FACE(toFace);

				BRep_Builder b;
				for each (IXWire ^ wire in wires)
				{
					const TopoDS_Wire& occWire = TOPO_WIRE(wire);
					b.Add(occFace, occWire);
				}
				return gcnew XFace(occFace);
			}

			System::Collections::Generic::IEnumerable<IXFace^>^ ShapeFactory::FixFace(IXFace^ face)
			{
				const TopoDS_Face& occFace = TOPO_FACE(face);

				TopTools_ListOfShape faces;
				ShapeExtend_Status status = OccHandle().FixFace(occFace, faces);
				List<IXFace^>^ faceList = gcnew List<IXFace^>();
				for (auto& faceIt = faces.cbegin(); faceIt != faces.cend(); faceIt++)
					faceList->Add(gcnew XFace(TopoDS::Face(*faceIt)));

				return faceList;
			}

			TopoDS_Shape ShapeFactory::Transform(TopoDS_Shape& shape, XbimMatrix3D matrix)
			{
				gp_Trsf trans = GEOMETRY_FACTORY->ToTransform(matrix);
				BRepBuilderAPI_Transform gTran(shape, trans, Standard_True);
				return gTran.Shape();
			}

		
			TopoDS_Shape ShapeFactory::BuildClosedShell(IIfcClosedShell^ closedShell)
			{
				CheckClosedStatus isCheckedClosed;
				TopoDS_Shell shell = SHELL_FACTORY->BuildClosedShell(closedShell, isCheckedClosed); //throws exeptions
				//ensure shells are discrete 
				Handle(ShapeFix_Solid) sfs = new ShapeFix_Solid(sfs->SolidFromShell(shell));
				sfs->FixShellMode() = true;
				sfs->FixShellOrientationMode() = true;
				sfs->Perform();
				return sfs->Shape();

			}
			

		}
	}
}