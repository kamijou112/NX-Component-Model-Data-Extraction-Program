//NX11_NXOpenCPP_Wizard3

// Mandatory UF Includes
#include <uf.h>
#include <uf_object_types.h>
#include <uf_ui.h>
#include <uf_modl.h>     // 特征建模相关函数
#include <uf_obj.h>	     // 对象操作相关函数
#include <uf_part.h>     // 部件操作相关函数

// Internal Includes
#include <NXOpen/ListingWindow.hxx>
#include <NXOpen/NXMessageBox.hxx>
#include <NXOpen/UI.hxx>

// Internal+External Includes
#include <NXOpen/Annotations.hxx>
#include <NXOpen/Assemblies_Component.hxx>
#include <NXOpen/Assemblies_ComponentAssembly.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/BodyCollection.hxx>
#include <NXOpen/Face.hxx>
#include <NXOpen/Line.hxx>
#include <NXOpen/NXException.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Features_BlockFeatureBuilder.hxx>
#include <NXOpen/Features_FeatureCollection.hxx> 
#include <NXOpen/Features_Extrude.hxx>
#include <NXOpen/Features_ExtrudeBuilder.hxx>
#include <NXOpen/Features_Revolve.hxx>
#include <NXOpen/Features_RevolveBuilder.hxx>
#include <NXOpen/Features_Chamfer.hxx>
#include <NXOpen/Features_ChamferBuilder.hxx>
#include <NXOpen/Features_Hole.hxx>
#include <NXOpen/Features_HolePackage.hxx>
#include <NXOpen/Features_HoleFeatureBuilder.hxx>
#include <NXOpen/Features_HolePackageBuilder.hxx>
#include <NXOpen/Features_Feature.hxx>
#include <NXOpen/Axis.hxx>          // 轴类定义
#include <NXOpen/Point.hxx>     
#include <NXOpen/PointCollection.hxx>
#include <NXOpen/Edge.hxx>
#include <NXOpen/Direction.hxx>
#include <NXOpen/DisplayManager.hxx>
#include <NXOpen/GeometricUtilities_Limits.hxx>
#include <NXOpen/GeometricUtilities_Extend.hxx>
#include <NXOpen/GeometricUtilities_FeatureOffset.hxx>
#include <NXOpen/GeometricUtilities_MultiDraft.hxx>
#include <NXOpen/Expression.hxx>
#include <NXOpen/ExpressionCollection.hxx>
#include <NXOpen/ICurve.hxx>
#include <NXOpen/Curve.hxx>
#include <NXOpen/Features_Feature.hxx>
#include <NXOpen/Section.hxx>
#include <NXOpen/SectionData.hxx>
#include <NXOpen/Features_SketchFeature.hxx>
#include <NXOpen/Features_Cylinder.hxx>
#include <NXOpen/Features_CylinderBuilder.hxx>
#include <NXOpen/Features_Block.hxx>
#include <NXOpen/Features_BlockFeatureBuilder.hxx>
#include <NXOpen/Features_DatumCsys.hxx>
#include <NXOpen/CartesianCoordinateSystem.hxx>
#include <NXOpen/Features_DatumCsysBuilder.hxx>
#include <NXOpen/NXMatrix.hxx>
#include <NXOpen/Sketch.hxx>
#include <NXOpen/Arc.hxx>
#include <NXOpen/SketchConstraint.hxx>
#include <NXOpen/SketchDimensionalConstraint.hxx>
#include <NXOpen/SketchGeometricConstraint.hxx>
#include <NXOpen/Annotations_Dimension.hxx>
#include <NXOpen/ISurface.hxx>
#include <NXOpen/Plane.hxx>
#include <NXOpen/DatumPlane.hxx>
#include <NXOpen/PlaneTypes.hxx> 



// Std C++ Includes
#include <iostream>
#include <sstream>
#include <vector>

using namespace NXOpen;
using std::string;
using std::exception;
using std::stringstream;
using std::endl;
using std::cout;
using std::cerr;




//------------------------------------------------------------------------------
// NXOpen c++ test class 
//------------------------------------------------------------------------------
class MyClass
{
    // class members
public:
    static Session *theSession;
    static UI *theUI;

    MyClass();
    ~MyClass();

	void do_it();
	void print(const NXString &);
	void print(const string &);
	void print(const char*);
	void extractFeatureData(NXOpen::Features::Feature* feature);
	void extractExtrudeData(NXOpen::Features::Feature* feature);
	void extractRevolveData(NXOpen::Features::Feature* feature);
	void extractChamferData(NXOpen::Features::Feature* feature);
	void extractCylinderData(NXOpen::Features::Feature* feature);
	void extractHoleData(NXOpen::Features::Feature* feature);
	void extractSketchData(NXOpen::Features::Feature* feature);
	void extractDatumCsysData(NXOpen::Features::Feature* features);
	void extractBlockData(NXOpen::Features::Feature* features);
	void printMatrix(const NXOpen::Matrix3x3& mat);
	void printPoint(const NXOpen::Point3d& point);

	std::string getBlockTypeAsString(NXOpen::Features::BlockFeatureBuilder::Types blocktype);
	std::string getExtendTypeAsString(NXOpen::GeometricUtilities::Extend::ExtendType extendType);
	std::string getRuleTypeAsString(NXOpen::SelectionIntentRule::RuleType ruleType);
	std::string getConstraintTypeString(NXOpen::Sketch::ConstraintType conType);
	std::string GetFaceTypeName(NXOpen::Face::FaceType type);
	std::string GetPlaneMethodName(NXOpen::PlaneTypes::MethodType method);
	std::string GetEdgeTypeName(NXOpen::Edge::EdgeType edgeType);

private:
	Part *workPart, *displayPart;
	NXMessageBox *mb;
	ListingWindow *lw;
	LogFile *lf;
};

//------------------------------------------------------------------------------
// Initialize static variables
//------------------------------------------------------------------------------
Session *(MyClass::theSession) = NULL;
UI *(MyClass::theUI) = NULL;

//------------------------------------------------------------------------------
// Constructor 
//------------------------------------------------------------------------------
MyClass::MyClass()
{

	// Initialize the NX Open C++ API environment
	MyClass::theSession = NXOpen::Session::GetSession();
	MyClass::theUI = UI::GetUI();
	mb = theUI->NXMessageBox();
	lw = theSession->ListingWindow();
	lf = theSession->LogFile();

	////返回当前工作部件（即当前活动的部件）。Part类表示一个NX部件，包含几何数据、特征等信息。theSession->Parts()：获取当前会话中的部件集合。
    workPart = theSession->Parts()->Work();
	displayPart = theSession->Parts()->Display();
	
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
MyClass::~MyClass()
{
}

//------------------------------------------------------------------------------
// Print string to listing window or stdout
//------------------------------------------------------------------------------
void MyClass::print(const NXString &msg)
{
	if(! lw->IsOpen() ) lw->Open();
	lw->WriteLine(msg);
}
void MyClass::print(const string &msg)
{
	if(! lw->IsOpen() ) lw->Open();
	lw->WriteLine(msg);
}
void MyClass::print(const char * msg)
{
	if(! lw->IsOpen() ) lw->Open();
	lw->WriteLine(msg);
}


void MyClass::extractFeatureData(NXOpen::Features::Feature* feature) {
	// 获取特征类型
	NXOpen::NXString featureType = feature->FeatureType();
	lw->WriteLine(feature->JournalIdentifier() + ":");
	string strType = featureType.GetLocaleText();
	lw->WriteLine("  type="+ featureType);
	std::vector<NXOpen::Features::Feature *> parentsFeatures = feature->GetParents();
	if (parentsFeatures.size() > 0) {
		for (auto x : parentsFeatures) {
			lw->WriteLine("  Parent=" + x->JournalIdentifier());
		}
	}
	std::vector<NXOpen::Features::Feature *> childrenFeatures = feature->GetChildren();
	if (childrenFeatures.size() > 0) {
		for (auto x : childrenFeatures) {
			lw->WriteLine("  Children=" + x->JournalIdentifier());
		}
	}

	if (strcmp(featureType.GetLocaleText(), "DATUM_CSYS") == 0) {
		extractDatumCsysData(feature); // 提取坐标系特征数据
	}
	else if (strcmp(featureType.GetLocaleText(), "SKETCH") == 0) {
		extractSketchData(feature); // 提取草图数据
	}
	else if (strcmp(featureType.GetLocaleText(), "EXTRUDE") == 0) {
		extractExtrudeData(feature); // 提取拉伸特征数据
	}
	else if (strcmp(featureType.GetLocaleText(), "SWP104") == 0) {
		extractRevolveData(feature); // 提取旋转特征数据
	}
	else if (strcmp(featureType.GetLocaleText(), "HOLE PACKAGE") == 0) {
		extractHoleData(feature); // 提取孔特征数据
	}
	else if (strcmp(featureType.GetLocaleText(), "CYLINDER") == 0) {
		extractCylinderData(feature); // 提取圆柱特征数据
	}
	else if (strcmp(featureType.GetLocaleText(), "BLOCK") == 0) {
		extractBlockData(feature); // 提取块特征数据
	}
	//else if (strType.find("HOLE") != std::string::npos) {
	//	extractHoleData(feature); // 提取孔特征数据
	//}
	else if (strcmp(featureType.GetLocaleText(), "CHAMFER") == 0) {
		extractChamferData(feature); // 提取倒角特征数据
	}
	//else if (strcmp(featureType, "BLEND") == 0) {
	//	extractBlendData(feature); // 提取圆角特征数据
	//}
	else {
		lw->WriteLine("  Unsupported feature type ");
	}
}

// 将扩展类型转换为字符串的函数
std::string MyClass::getBlockTypeAsString(NXOpen::Features::BlockFeatureBuilder::Types blocktype) {
	switch (blocktype) {
	case NXOpen::Features::BlockFeatureBuilder::TypesOriginAndEdgeLengths:
		return "Origin and Edge Lengths";
	case NXOpen::Features::BlockFeatureBuilder::TypesTwoPointsAndHeight:
		return "Two Points and Height";
	case NXOpen::Features::BlockFeatureBuilder::TypesDiagonalPoints:
		return "Diagonal Points";
	default:
		return "Unknown";
	}
}

// 将扩展类型转换为字符串的函数
std::string MyClass::getExtendTypeAsString(NXOpen::GeometricUtilities::Extend::ExtendType extendType) {
	switch (extendType) {
	case NXOpen::GeometricUtilities::Extend::ExtendType::ExtendTypeValue:
		return "Value";
	case NXOpen::GeometricUtilities::Extend::ExtendType::ExtendTypeUntilNext:
		return "Until Next";
	case NXOpen::GeometricUtilities::Extend::ExtendType::ExtendTypeUntilSelected:
		return "Until Selected";
	case NXOpen::GeometricUtilities::Extend::ExtendType::ExtendTypeUntilExtended:
		return "Until Extended";
	case NXOpen::GeometricUtilities::Extend::ExtendType::ExtendTypeThroughAll:
		return "Through All";
	case NXOpen::GeometricUtilities::Extend::ExtendType::ExtendTypeSymmetric:
		return "Symmetric";
	case NXOpen::GeometricUtilities::Extend::ExtendType::ExtendTypePercent:
		return "Percent";
	case NXOpen::GeometricUtilities::Extend::ExtendType::ExtendTypeArcLength:
		return "Arc Length";
	case NXOpen::GeometricUtilities::Extend::ExtendType::ExtendTypeThruPoint:
		return "Thru Point";
	default:
		return "Unknown";
	}
}

// 将选择意图规则的类型转换为字符串
std::string MyClass::getRuleTypeAsString(NXOpen::SelectionIntentRule::RuleType ruleType) {
	switch (ruleType) {
	case NXOpen::SelectionIntentRule::RuleTypeEdgeDumb: return "EdgeDumb";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeChain: return "EdgeChain";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeTangent: return "EdgeTangent";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeFace: return "EdgeFace";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeBody: return "EdgeBody";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeFeature: return "EdgeFeature";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeSheetBoundary: return "EdgeSheetBoundary";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeBoundary: return "EdgeBoundary";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeIntersect: return "EdgeIntersect";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeMultipleSeedTangent: return "EdgeMultipleSeedTangent";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeVertex: return "EdgeVertex";
	case NXOpen::SelectionIntentRule::RuleTypeEdgeVertexTangent: return "EdgeVertexTangent";
	case NXOpen::SelectionIntentRule::RuleTypeCurveDumb: return "CurveDumb";
	case NXOpen::SelectionIntentRule::RuleTypeCurveFeature: return "CurveFeature";
	case NXOpen::SelectionIntentRule::RuleTypeCurveFeatureChain: return "CurveFeatureChain";
	case NXOpen::SelectionIntentRule::RuleTypeCurveFeatureTangent: return "CurveFeatureTangent";
	case NXOpen::SelectionIntentRule::RuleTypeFollowFillet: return "FollowFillet";
	case NXOpen::SelectionIntentRule::RuleTypeFeaturePoints: return "FeaturePoints";
	case NXOpen::SelectionIntentRule::RuleTypeRegionBoundary: return "RegionBoundary";
	case NXOpen::SelectionIntentRule::RuleTypeFaceDumb: return "FaceDumb";
	case NXOpen::SelectionIntentRule::RuleTypeFaceTangent: return "FaceTangent";
	case NXOpen::SelectionIntentRule::RuleTypeFaceAdjacent: return "FaceAdjacent";
	case NXOpen::SelectionIntentRule::RuleTypeFaceBody: return "FaceBody";
	case NXOpen::SelectionIntentRule::RuleTypeFaceRegion: return "FaceRegion";
	case NXOpen::SelectionIntentRule::RuleTypeFaceFeature: return "FaceFeature";
	case NXOpen::SelectionIntentRule::RuleTypeFaceConnectedBlend: return "FaceConnectedBlend";
	case NXOpen::SelectionIntentRule::RuleTypeFaceAllBlend: return "FaceAllBlend";
	case NXOpen::SelectionIntentRule::RuleTypeFaceRib: return "FaceRib";
	case NXOpen::SelectionIntentRule::RuleTypeFaceMergedRib: return "FaceMergedRib";
	case NXOpen::SelectionIntentRule::RuleTypeFaceSlot: return "FaceSlot";
	case NXOpen::SelectionIntentRule::RuleTypeFaceBossPocket: return "FaceBossPocket";
	case NXOpen::SelectionIntentRule::RuleTypeFaceRegionBoundary: return "FaceRegionBoundary";
	case NXOpen::SelectionIntentRule::RuleTypeFaceAndAdjacentFaces: return "FaceAndAdjacentFaces";
	case NXOpen::SelectionIntentRule::RuleTypeCurveGroup: return "CurveGroup";
	case NXOpen::SelectionIntentRule::RuleTypeBodyDumb: return "BodyDumb";
	case NXOpen::SelectionIntentRule::RuleTypeBodyFeature: return "BodyFeature";
	case NXOpen::SelectionIntentRule::RuleTypeBodyGroup: return "BodyGroup";
	case NXOpen::SelectionIntentRule::RuleTypeApparentChaining: return "ApparentChaining";
	case NXOpen::SelectionIntentRule::RuleTypeOuterFaceEdges: return "OuterFaceEdges";
	case NXOpen::SelectionIntentRule::RuleTypeRibTopFaceEdges: return "RibTopFaceEdges";
	case NXOpen::SelectionIntentRule::RuleTypeFeatureIntersectionEdges: return "FeatureIntersectionEdges";
	default: return "InvalidType";
	}
}

// 约束类型转换函数
std::string MyClass::getConstraintTypeString(NXOpen::Sketch::ConstraintType conType) {
	switch (conType) {
	case NXOpen::Sketch::ConstraintTypeFixed:               return "Fixed";
	case NXOpen::Sketch::ConstraintTypeHorizontal:          return "Horizontal";
	case NXOpen::Sketch::ConstraintTypeVertical:            return "Vertical";
	case NXOpen::Sketch::ConstraintTypeParallel:            return "Parallel";
	case NXOpen::Sketch::ConstraintTypePerpendicular:       return "Perpendicular";
	case NXOpen::Sketch::ConstraintTypeCollinear:           return "Collinear";
	case NXOpen::Sketch::ConstraintTypeEqualLength:         return "Equal Length";
	case NXOpen::Sketch::ConstraintTypeEqualRadius:         return "Equal Radius";
	case NXOpen::Sketch::ConstraintTypeConstantLength:      return "Constant Length";
	case NXOpen::Sketch::ConstraintTypeConstantAngle:       return "Constant Angle";
	case NXOpen::Sketch::ConstraintTypeCoincident:          return "Coincident";
	case NXOpen::Sketch::ConstraintTypeConcentric:          return "Concentric";
	case NXOpen::Sketch::ConstraintTypeMirror:              return "Mirror";
	case NXOpen::Sketch::ConstraintTypePointOnCurve:        return "Point On Curve";
	case NXOpen::Sketch::ConstraintTypeMidpoint:            return "Midpoint";
	case NXOpen::Sketch::ConstraintTypeRadiusDim:           return "Radius Dim";
	case NXOpen::Sketch::ConstraintTypeTangent:				return "Tangent";
	case NXOpen::Sketch::ConstraintTypeHorizontalDim:       return "Horizontal Dim";
	case NXOpen::Sketch::ConstraintTypeVerticalDim:         return "Vertical Dim";
	case NXOpen::Sketch::ConstraintTypeParallelDim:         return "Parallel Dim";
	case NXOpen::Sketch::ConstraintTypePerpendicularDim:    return "Perpendicular Dim";
	case NXOpen::Sketch::ConstraintTypeAngularDim:          return "Angular Dim";
	case NXOpen::Sketch::ConstraintTypePointOnString:       return "Point On String";
	case NXOpen::Sketch::ConstraintTypeSlope:               return "Slope";
	case NXOpen::Sketch::ConstraintTypePerimeterDim:        return "Perimeter Dim";
	case NXOpen::Sketch::ConstraintTypeOffset:              return "Offset";
	case NXOpen::Sketch::ConstraintTypeNormal:              return "Normal";
	case NXOpen::Sketch::ConstraintTypePointOnLoop:         return "Point On Loop";
	case NXOpen::Sketch::ConstraintTypeRecipeTrim:          return "Recipe Trim";
	case NXOpen::Sketch::ConstraintTypePattern:             return "Pattern";
		// 添加更多约束类型...
	default:                                return "Unknown (" + std::to_string(conType) + ")";
	}
}

//面类型转换
std::string MyClass::GetFaceTypeName(NXOpen::Face::FaceType type) {
	switch (type) {
	case NXOpen::Face::FaceTypeRubber:
		return "Rubber"; // 橡皮面（无实际几何）
	case NXOpen::Face::FaceTypePlanar:
		return "Planar"; // 平面
	case NXOpen::Face::FaceTypeCylindrical:
		return "Cylindrical"; // 圆柱面
	case NXOpen::Face::FaceTypeConical:
		return "Conical"; // 圆锥面
	case NXOpen::Face::FaceTypeSpherical:
		return "Spherical"; // 球面
	case NXOpen::Face::FaceTypeSurfaceOfRevolution:
		return "SurfaceOfRevolution"; // 旋转曲面
	case NXOpen::Face::FaceTypeParametric:
		return "Parametric"; // 参数化曲面
	case NXOpen::Face::FaceTypeBlending:
		return "Blending"; // 过渡面（如倒圆角）
	case NXOpen::Face::FaceTypeOffset:
		return "Offset"; // 偏移面
	case NXOpen::Face::FaceTypeSwept:
		return "Swept"; // 扫掠面
	case NXOpen::Face::FaceTypeUndefined:
		return "Undefined"; // 未定义类型
	default:
		return "Unknown"; // 未知类型（兼容未来扩展）
	}
}

//平面类型名称转换
std::string MyClass::GetPlaneMethodName(NXOpen::PlaneTypes::MethodType method) {
	switch (method) {
	
	default:
		return "Unknown";              // 未知类型（兼容未来扩展）
	}
}

//边类型转换
std::string MyClass::GetEdgeTypeName(NXOpen::Edge::EdgeType edgeType) {
	switch (edgeType) {
	case NXOpen::Edge::EdgeTypeRubber:
		return "Rubber";            // 橡皮边（无实际曲线）
	case NXOpen::Edge::EdgeTypeLinear:
		return "Linear";           // 直线边
	case NXOpen::Edge::EdgeTypeCircular:
		return "Circular";          // 圆弧边
	case NXOpen::Edge::EdgeTypeElliptical:
		return "Elliptical";        // 椭圆弧边
	case NXOpen::Edge::EdgeTypeIntersection:
		return "Intersection";     // 相交边（两曲面交线）
	case NXOpen::Edge::EdgeTypeSpline:
		return "Spline";           // 样条边
	case NXOpen::Edge::EdgeTypeSpCurve:
		return "SpCurve";           // SP曲线边（特殊曲线）
	case NXOpen::Edge::EdgeTypeForeign:
		return "Foreign";           // 外部导入边（非NX原生）
	case NXOpen::Edge::EdgeTypeConstantParameter:
		return "ConstantParameter"; // 等参数边（如UV线）
	case NXOpen::Edge::EdgeTypeTrimmedCurve:
		return "TrimmedCurve";      // 修剪曲线边
	case NXOpen::Edge::EdgeTypeUndefined:
		return "Undefined";         // 未定义类型
	default:
		return "Unknown";           // 未知类型（兼容未来扩展）
	}
}

//圆柱提取
void MyClass::extractCylinderData(NXOpen::Features::Feature* feature) {
	NXOpen::Features::Cylinder* cylinderFeature = dynamic_cast<NXOpen::Features::Cylinder*>(feature);
	if (cylinderFeature) {
		// 创建构建器
		NXOpen::Features::CylinderBuilder* cylinderBuilder = workPart->Features()->CreateCylinderBuilder(cylinderFeature);
		lw->WriteLine("	Cylinder Data:");
		Point3d origin = cylinderBuilder->Origin();

		// 输出变换原点
		lw->WriteLine("	Origin=("
			+ std::to_string(origin.X) + ", "
			+ std::to_string(origin.Y) + ", "
			+ std::to_string(origin.Z) + ")");

		// 2. 法向量
		NXOpen::Vector3d direction = cylinderBuilder->Direction();
		lw->WriteLine("	Direction=(" +
			std::to_string(direction.X) + ", " +
			std::to_string(direction.Y) + ", " +
			std::to_string(direction.Z) + ")");

		lw->WriteLine("	Diameter=" + std::to_string(cylinderBuilder->Diameter()->Value()));
		lw->WriteLine("	Height=" + std::to_string(cylinderBuilder->Height()->Value()));

		// 获取布尔操作类型
		NXOpen::GeometricUtilities::BooleanOperation::BooleanType booleanType = cylinderBuilder->BooleanOption()->Type();

		std::string booleanTypeStr;
		switch (int(booleanType)) {
		case 0: booleanTypeStr = "None"; break;  // 无
		case 1: booleanTypeStr = "Unite"; break;   // 合并
		case 2: booleanTypeStr = "Subtract"; break; // 减去
		case 3: booleanTypeStr = "Intersect"; break; // 相交
		default: booleanTypeStr = "Unknown"; break;
		}

		// 获取目标体
		std::vector<NXOpen::Body*> targetBodies = cylinderBuilder->BooleanOption()->GetTargetBodies();
		// 输出布尔操作信息
		lw->WriteLine("  Boolean Operation Data:");
		lw->WriteLine("    Boolean Type: " + booleanTypeStr);
		lw->WriteLine("    Number of Target Bodies: " + std::to_string(targetBodies.size()));
		// 输出目标体的标签
		for (size_t i = 0; i < targetBodies.size(); ++i) {
			NXOpen::Body* body = targetBodies[i];
			lw->WriteLine("    Target Body " + std::to_string(i + 1) + ": JournalIdentifier = " + body->JournalIdentifier());
		}
	}
	else {
		lw->WriteLine("Failed to cast feature to Cylinder.");
	}
}

void MyClass::extractBlockData(NXOpen::Features::Feature* feature) {
	NXOpen::Features::Block* blockFeature = dynamic_cast<NXOpen::Features::Block*>(feature);
	if (blockFeature) {
		// 创建构建器
		NXOpen::Features::BlockFeatureBuilder* blockBuilder = workPart->Features()->CreateBlockFeatureBuilder(blockFeature);
		lw->WriteLine("	Block Data:");
		lw->WriteLine("	Type=" + getBlockTypeAsString(blockBuilder->Type()));

		Point3d origin = blockBuilder->Origin();

		// 输出变换原点
		lw->WriteLine("	Origin=("
			+ std::to_string(origin.X) + ", "
			+ std::to_string(origin.Y) + ", "
			+ std::to_string(origin.Z) + ")");


		lw->WriteLine("	Length=" + std::to_string(blockBuilder->Length()->Value()));
		lw->WriteLine("	Width=" + std::to_string(blockBuilder->Width()->Value()));
		lw->WriteLine("	Height=" + std::to_string(blockBuilder->Height()->Value()));

		// 获取布尔操作类型
		NXOpen::GeometricUtilities::BooleanOperation::BooleanType booleanType = blockBuilder->BooleanOption()->Type();

		std::string booleanTypeStr;
		switch (int(booleanType)) {
		case 0: booleanTypeStr = "None"; break;  // 无
		case 1: booleanTypeStr = "Unite"; break;   // 合并
		case 2: booleanTypeStr = "Subtract"; break; // 减去
		case 3: booleanTypeStr = "Intersect"; break; // 相交
		default: booleanTypeStr = "Unknown"; break;
		}

		// 获取目标体
		std::vector<NXOpen::Body*> targetBodies = blockBuilder->BooleanOption()->GetTargetBodies();
		// 输出布尔操作信息
		lw->WriteLine("  Boolean Operation Data:");
		lw->WriteLine("    Boolean Type: " + booleanTypeStr);
		lw->WriteLine("    Number of Target Bodies: " + std::to_string(targetBodies.size()));
		// 输出目标体的标签
		for (size_t i = 0; i < targetBodies.size(); ++i) {
			NXOpen::Body* body = targetBodies[i];
			lw->WriteLine("    Target Body " + std::to_string(i + 1) + ": JournalIdentifier = " + body->JournalIdentifier());
		}
	}
	else {
		lw->WriteLine("Failed to cast feature to Cylinder.");
	}
}


//坐标系数据提取
void MyClass::extractDatumCsysData(NXOpen::Features::Feature* feature) {
	Features::DatumCsys* csysFeature = dynamic_cast<Features::DatumCsys*>(feature);
	if (csysFeature) {
		// 创建特征构建器
		NXOpen::Features::DatumCsysBuilder* csysBuilder = workPart->Features()->CreateDatumCsysBuilder(csysFeature);
		NXOpen::CartesianCoordinateSystem* datumCsys = csysBuilder->Csys();
		lw->WriteLine("  Datum CSYS Data:");
		bool fixedSizeDatum = csysBuilder->FixedSizeDatum();
		lw->WriteLine("    fixed size flag:"+ std::to_string(fixedSizeDatum));
		lw->WriteLine("    Scale factor:" + std::to_string(csysBuilder->DisplayScaleFactor()));

		//lw->WriteLine("缩放系数: " + std::to_string(scale));

		// 获取坐标系矩阵
		NXOpen::Matrix3x3 matrix = datumCsys->Orientation()->Element();
		Point3d origin = datumCsys->Origin();

		// 输出变换原点
		lw->WriteLine("    Origin: ("
			+ std::to_string(origin.X) + ", "
			+ std::to_string(origin.Y) + ", "
			+ std::to_string(origin.Z) + ")");

		

		bool isIdentity = (matrix.Xx == 1 && matrix.Xy == 0 && matrix.Xz == 0 &&
			matrix.Yx == 0 && matrix.Yy == 1 && matrix.Yz == 0 &&
			matrix.Zx == 0 && matrix.Zy == 0 && matrix.Zz == 1);

		//// 检查是否为世界坐标系
		//if (isIdentity &&  //是否为单位矩阵
		//	origin.X == 0 && origin.Y == 0 && origin.Z == 0) {                        //单位矩阵表征坐标系未发生旋转变换或缩放变换的基准状态，而原点坐标全零表明未发生平移偏移
		//	lw->WriteLine("    Type: World Coordinate System");
		//}
		//else {
		//	lw->WriteLine("    Type: User-defined Coordinate System");
		//
		//}
		// 输出变换矩阵
		lw->WriteLine("    Transformation Matrix:");
		lw->WriteLine("      [ " +
			std::to_string(matrix.Xx) + ", " +
			std::to_string(matrix.Xy) + ", " +
			std::to_string(matrix.Xz) + ", " +
			std::to_string(origin.X) + " ]");
		lw->WriteLine("      [ " +
			std::to_string(matrix.Yx) + ", " +
			std::to_string(matrix.Yy) + ", " +
			std::to_string(matrix.Yz) + ", " +
			std::to_string(origin.Y) + " ]");
		lw->WriteLine("      [ " +
			std::to_string(matrix.Zx) + ", " +
			std::to_string(matrix.Zy) + ", " +
			std::to_string(matrix.Zz) + ", " +
			std::to_string(origin.Z) + " ]");
		lw->WriteLine("      [ 0.000000, 0.000000, 0.000000, 1.000000 ]");

		// 释放资源
		csysBuilder->Destroy();
	}
	else {
		lw->WriteLine("Failed to cast feature to DatumCsys.");
	}
}

// 草图数据提取实现
void MyClass::extractSketchData(NXOpen::Features::Feature* feature) {
	Features::SketchFeature* sketchFeature = dynamic_cast<Features::SketchFeature*>(feature);
	if (sketchFeature) {
		Sketch* sketch = sketchFeature->Sketch();
		lw->WriteLine("  Sketch Data:");

		Point3d origin = sketch->Origin();
		lw->WriteLine("    Origin: (" +
			std::to_string(origin.X) + ", " +
			std::to_string(origin.Y) + ", " +
			std::to_string(origin.Z) + ")");
		NXOpen::NXMatrix* orientation = sketch->Orientation();
		NXOpen::Matrix3x3 matrix = orientation->Element();
		lw->WriteLine("    Orientation Matrix:");
		lw->WriteLine("      [" + std::to_string(matrix.Xx) + ", " + 
			std::to_string(matrix.Xy) + ", " + 
			std::to_string(matrix.Xz) + "]");
		lw->WriteLine("      [" + std::to_string(matrix.Yx) + 
			", " + std::to_string(matrix.Yy) + 
			", " + std::to_string(matrix.Yz) + "]");
		lw->WriteLine("      [" + std::to_string(matrix.Zx) + 
			", " + std::to_string(matrix.Zy) + 
			", " + std::to_string(matrix.Zz) + "]");

		//  其他属性
		NXOpen::ISurface* obj = sketch->AttachPlane();
		lw->WriteLine("    Attached Plane Data: " + sketch->AttachPlane()->JournalIdentifier()); //the plane that the sketch is attached to.   
		if (NXOpen::Plane* plane = dynamic_cast<NXOpen::Plane*>(obj)) {
			// 处理几何平面
			lw->WriteLine("      Type: Plane");

			// 1. 原点坐标
			NXOpen::Point3d origin = plane->Origin();
			lw->WriteLine("      Origin: (" +
				std::to_string(origin.X) + ", " +
				std::to_string(origin.Y) + ", " +
				std::to_string(origin.Z) + ")");

			// 2. 法向量
			NXOpen::Vector3d normal = plane->Normal();
			lw->WriteLine("      Normal: (" +
				std::to_string(normal.X) + ", " +
				std::to_string(normal.Y) + ", " +
				std::to_string(normal.Z) + ")");
			// 3. 平面创建方法 (如三点、系数等)
			NXOpen::PlaneTypes::MethodType method = plane->Method();
			//lw->WriteLine("        Method: " + GetPlaneMethodName(method)); // 自定义方法名
		}
		else if (NXOpen::Face* face = dynamic_cast<NXOpen::Face*>(obj)) {
			// 处理实体面
			lw->WriteLine("      Type: Face");

			// 1. 面类型（平面、圆柱、球面等）
			NXOpen::Face::FaceType faceType = face->SolidFaceType();
			lw->WriteLine("      Face Type: " + GetFaceTypeName(faceType)); // 自定义类型名转换

			// 获取面的所有边
			std::vector<NXOpen::Edge*> edges = face->GetEdges();

			lw->WriteLine("      Face Edges (" + std::to_string(edges.size()) + "):");
			for (size_t i = 0; i < edges.size(); ++i) {
				NXOpen::Edge* edge = edges[i];
				if (!edge) continue;

				// 基础信息

				double length = edge->GetLength();

				// 顶点坐标
				NXOpen::Point3d v1, v2;
				edge->GetVertices(&v1, &v2);

				// 边类型
				NXOpen::Edge::EdgeType edgeType = edge->SolidEdgeType();
				std::string typeName = GetEdgeTypeName(edgeType);

				// 按指定格式输出
				lw->WriteLine("      Edge " + std::to_string(i + 1) + ":");
				lw->WriteLine("        Type: " + typeName);
				lw->WriteLine("        Length: " + std::to_string(length));
				lw->WriteLine("        Vertex1: (" +
					std::to_string(v1.X) + ", " +
					std::to_string(v1.Y) + ", " +
					std::to_string(v1.Z) + ")");
				lw->WriteLine("        Vertex2: (" +
					std::to_string(v2.X) + ", " +
					std::to_string(v2.Y) + ", " +
					std::to_string(v2.Z) + ")");

			}
		
			//// 4. 关联的体（父级实体）
			//NXOpen::Body* body = face->GetBody();
			//if (body) {
			//	lw->WriteLine("        Body Name: " + body->Name());
			//}


		}
		else if (NXOpen::DatumPlane* datumPlane = dynamic_cast<NXOpen::DatumPlane*>(obj)) {
			// 处理基准平面
			lw->WriteLine("      Type: DatumPlane");

			// 1. 原点坐标
			NXOpen::Point3d origin = datumPlane->Origin();
			lw->WriteLine("      Origin: (" +
				std::to_string(origin.X) + ", " +
				std::to_string(origin.Y) + ", " +
				std::to_string(origin.Z) + ")");

			// 2. 法向量
			NXOpen::Vector3d normal = datumPlane->Normal();
			lw->WriteLine("      Normal: (" +
				std::to_string(normal.X) + ", " +
				std::to_string(normal.Y) + ", " +
				std::to_string(normal.Z) + ")");
		}
		

		lw->WriteLine("    Is Internal: " + std::string(sketch->IsInternal() ? "True" : "False"));
	

		// 几何元素提取
		std::vector<NXOpen::NXObject*> allGeometry = sketch->GetAllGeometry();
		lw->WriteLine("    Geometry Count: " + std::to_string(allGeometry.size()));

		for (size_t i = 0; i < allGeometry.size(); ++i) {
			NXOpen::DisplayableObject* geom = dynamic_cast<NXOpen::DisplayableObject*>(allGeometry[i]);
			if (!geom) continue;

			NXString typeName = geom->Name();
			lw->WriteLine("    Geometry " + std::to_string(i + 1) + ": " + typeName);

			// 线段处理
			if (NXOpen::Line* line = dynamic_cast<NXOpen::Line*>(geom)) {
				NXOpen::Point3d start = line->StartPoint();
				NXOpen::Point3d end = line->EndPoint();
				lw->WriteLine("      Start: (" +
					std::to_string(start.X) + ", " +
					std::to_string(start.Y) + ", " +
					std::to_string(start.Z) + ")");
				lw->WriteLine("      End: (" +
					std::to_string(end.X) + ", " +
					std::to_string(end.Y) + ", " +
					std::to_string(end.Z) + ")");
			}
			// 圆弧处理
			else if (NXOpen::Arc* arc = dynamic_cast<NXOpen::Arc*>(geom)) {
				NXOpen::Point3d center = arc->CenterPoint();
				double radius = arc->Radius();
				double startAngle = arc->StartAngle();
				double endAngle = arc->EndAngle();
				lw->WriteLine("      Center: (" +
					std::to_string(center.X) + ", " +
					std::to_string(center.Y) + ", " +
					std::to_string(center.Z) + ")");
				lw->WriteLine("      Radius: " + std::to_string(radius));
				lw->WriteLine("      StartAngle: " + std::to_string(startAngle));
				lw->WriteLine("      EndAngle: " + std::to_string(endAngle));
			}
			else if (NXOpen::Point* point = dynamic_cast<NXOpen::Point*>(geom)) {
				NXOpen::Point3d center = point->Coordinates();
				lw->WriteLine("      Coordinates: (" +
					std::to_string(center.X) + ", " +
					std::to_string(center.Y) + ", " +
					std::to_string(center.Z) + ")");
			}
		}

		// 4. 约束信息提取
		// ---------------------------
		std::vector<NXOpen::SketchConstraint*> constraints =
			sketch->GetAllConstraintsOfType(
				NXOpen::Sketch::ConstraintClassAny,
				NXOpen::Sketch::ConstraintTypeNoCon
			);

		lw->WriteLine("    Total Constraints: " + std::to_string(constraints.size()));

		for (auto con : constraints) {
			NXOpen::Sketch::ConstraintType conType = con->ConstraintType();
			std::string conTypeStr = getConstraintTypeString(conType);
			lw->WriteLine("    Constraint Type: " + conTypeStr);

			// 处理尺寸约束
			if (NXOpen::SketchDimensionalConstraint* dimCon =
				dynamic_cast<NXOpen::SketchDimensionalConstraint*>(con))
			{
				// 获取尺寸值
				double value = dimCon->AssociatedDimension()->ComputedSize();
				lw->WriteLine("      Value: " + std::to_string(value));

				// 获取关联几何
				std::vector<NXOpen::Sketch::DimensionGeometry> dimGeoms =
					dimCon->GetDimensionGeometry();

				lw->WriteLine("      Associated Geometry:");
				for (size_t i = 0; i < dimGeoms.size(); ++i) {
					NXOpen::NXObject* geom = dimGeoms[i].Geometry;
					if (geom) {
						std::string geomName = geom->Name().GetLocaleText();
						lw->WriteLine("        Name:" + geomName);
					}
				}
			}
			// 处理几何约束
			else if (NXOpen::SketchGeometricConstraint* geoCon = dynamic_cast<NXOpen::SketchGeometricConstraint*>(con))
			{
				// 获取约束关联的几何元素
				std::vector< NXOpen::Sketch::ConstraintGeometry> geoms = geoCon->GetGeometry();

				lw->WriteLine("    Associated Geometry (" + std::to_string(geoms.size()) + "):");
				for (size_t i = 0; i < geoms.size(); ++i) {
					NXOpen::NXObject* geom = geoms[i].Geometry;
					if (geom) {
						std::string geomName = geom->Name().GetLocaleText();
						lw->WriteLine("      Geometry " + std::to_string(i + 1) + ": Name="  + geomName);
					}
				}
			}
		}


	}
	else {
		lw->WriteLine("Failed to cast feature to Sketch.");
	}
}

void MyClass::extractExtrudeData(NXOpen::Features::Feature* feature) {
	// 将特征转换为拉伸特征
	NXOpen::Features::Extrude* extrudeFeature = dynamic_cast<NXOpen::Features::Extrude*>(feature);
	if (extrudeFeature) {
		// 创建拉伸特征的构建器
		NXOpen::Features::ExtrudeBuilder* extrudeBuilder = workPart->Features()->CreateExtrudeBuilder(extrudeFeature);

		// 获取拉伸方向
		NXOpen::Direction* direction = extrudeBuilder->Direction();
		NXOpen::Vector3d dirVector = direction->Vector();

		// 获取拉伸的起始和结束类型和距离
		NXOpen::Expression* startExpression = extrudeBuilder->Limits()->StartExtend()->Value();
		NXOpen::Expression* endExpression = extrudeBuilder->Limits()->EndExtend()->Value();

		NXOpen::GeometricUtilities::Extend::ExtendType startType = extrudeBuilder->Limits()->StartExtend()->TrimType();
		NXOpen::GeometricUtilities::Extend::ExtendType endType = extrudeBuilder->Limits()->EndExtend()->TrimType();
		double startDistance = startExpression->Value();
		double endDistance = endExpression->Value();


		// 将扩展类型转换为字符串
		std::string startTypeStr = getExtendTypeAsString(startType);
		std::string endTypeStr = getExtendTypeAsString(endType);


		// 获取布尔操作类型
		NXOpen::GeometricUtilities::BooleanOperation::BooleanType booleanType = extrudeBuilder->BooleanOperation()->Type();
		
		std::string booleanTypeStr;
		switch (int(booleanType)) {
		case 0: booleanTypeStr = "None"; break;  // 无
		case 1: booleanTypeStr = "Unite"; break;   // 合并
		case 2: booleanTypeStr = "Subtract"; break; // 减去
		case 3: booleanTypeStr = "Intersect"; break; // 相交
		default: booleanTypeStr = "Unknown"; break;
		}

		// 获取目标体
		std::vector<NXOpen::Body*> targetBodies = extrudeBuilder->BooleanOperation()->GetTargetBodies();

		// 获取偏置信息
		bool hasOffset = extrudeBuilder->Offset()->Option();
		double startOffset = 0.0, endOffset = 0.0;
		if (hasOffset) {
			startOffset = extrudeBuilder->Offset()->StartOffset()->Value();
			endOffset = extrudeBuilder->Offset()->EndOffset()->Value();
		}

		// 获取拔模信息
		bool hasDraft = extrudeBuilder->Draft()->DraftOption();
		double draftAngle = 0.0;
		if (hasDraft) {
			draftAngle = extrudeBuilder->Draft()->GetAngleOption();
		}

		// 获取拉伸特征的截面对象
		NXOpen::Section* section = extrudeBuilder->Section();
		// 获取截面数据
		std::vector<NXOpen::SectionData*> sectionData;
		section->GetSectionData(sectionData);

		// 输出截面数据
		lw->WriteLine("  Extrude Section Data:");
		for (size_t i = 0; i < sectionData.size(); ++i) {
			NXOpen::SectionData* data = sectionData[i];
			lw->WriteLine("    Section Data " + std::to_string(i + 1) + ":");

			// 获取并输出起始和结束连接器
			NXOpen::NXObject* startConnector = data->GetStartConnector();
			NXOpen::NXObject* endConnector = data->GetEndConnector();
			if (startConnector) {
				lw->WriteLine("      Start Connector: " + startConnector->Name());
			}
			if (endConnector) {
				lw->WriteLine("      End Connector: " + endConnector->Name());
			}

			//// 获取并输出选择意图规则
			//std::vector<NXOpen::SelectionIntentRule*> rules;
			//data->GetRules(rules);
			//for (size_t j = 0; j < rules.size(); ++j) {
			//	NXOpen::SelectionIntentRule::RuleType ruleType = rules[j]->Type();
			//	lw->WriteLine("    Rule " + std::to_string(j + 1) + ": " + getRuleTypeAsString(ruleType));
			//}

			// 获取并输出截面元素数据
			std::vector<NXOpen::SectionElementData*> sectionElementsData;
			data->GetSectionElementsData(sectionElementsData);
			for (size_t j = 0; j < sectionElementsData.size(); ++j) {
				NXOpen::SectionElementData* elementData = sectionElementsData[j];
				lw->WriteLine("    Section Element Data " + std::to_string(j + 1) + ":");

				NXOpen::DisplayableObject* sectionElement;
				NXOpen::DisplayableObject* startConnectorElement;
				NXOpen::Point3d startPoint;
				NXOpen::DisplayableObject* endConnectorElement;
				NXOpen::Point3d endPoint;

				elementData->GetSectionElementData1(&sectionElement, &startConnectorElement, &startPoint, &endConnectorElement, &endPoint);

				if (sectionElement) {
					lw->WriteLine("      Section Element: " + sectionElement->Name());
				}
				// 检查是否为圆弧 ------------------------------------------------------
				if (NXOpen::Arc* arc = dynamic_cast<NXOpen::Arc*>(sectionElement)) {
					// 获取圆心和半径
					NXOpen::Point3d center = arc->CenterPoint();
					double radius = arc->Radius();

					// 格式化输出
					lw->WriteLine("      Type: Arc");
					lw->WriteLine("      Center: (" +
						std::to_string(center.X) + ", " +
						std::to_string(center.Y) + ", " +
						std::to_string(center.Z) + ")");
					lw->WriteLine("      Radius: " + std::to_string(radius));
				}
				// 检查是否为其他类型（如直线）
				else if (NXOpen::Line* line = dynamic_cast<NXOpen::Line*>(sectionElement)) {
					lw->WriteLine("      Type: Line");
				}
				// 其他类型（如样条、点等）
				else {
					lw->WriteLine("      Type: Unknown/Generic Curve");
				}
				if (startConnectorElement) {
					lw->WriteLine("      Start Connector Element: " + startConnectorElement->Name());
				}
				lw->WriteLine("      Start Point: (" + std::to_string(startPoint.X) + ", " + std::to_string(startPoint.Y) + ", " + std::to_string(startPoint.Z) + ")");
				if (endConnectorElement) {
					lw->WriteLine("      End Connector Element: " + endConnectorElement->Name());
				}
				lw->WriteLine("      End Point: (" + std::to_string(endPoint.X) + ", " + std::to_string(endPoint.Y) + ", " + std::to_string(endPoint.Z) + ")");
			
			}
		}

		lw->WriteLine("  Direction: (" + std::to_string(dirVector.X) + ", " + std::to_string(dirVector.Y) + ", " + std::to_string(dirVector.Z) + ")");


		
		if (strcmp(startTypeStr.c_str(), "Value") == 0) {//是值则输出起始距离
			lw->WriteLine("  Start Type: " + startTypeStr);
			lw->WriteLine("  Start Distance: " + std::to_string(startDistance));
		}
		else if (strcmp(startTypeStr.c_str(), "Symmetric") == 0) {//是对称值则输出结束类型和距离
			endTypeStr = getExtendTypeAsString(startType);
			lw->WriteLine("  End Type: " + endTypeStr);
			lw->WriteLine("  End Distance: " + std::to_string(endDistance));
		}
		else {
			lw->WriteLine("  Start Type: " + startTypeStr);
		}

		if (strcmp(endTypeStr.c_str(), "Symmetric") != 0) {
			lw->WriteLine("  End Type: " + endTypeStr);
		}
		if (strcmp(endTypeStr.c_str(), "Value") == 0) {
			lw->WriteLine("  End Distance: " + std::to_string(endDistance));
		}
		
		// 输出布尔操作信息
		lw->WriteLine("  Boolean Operation Data:");
		lw->WriteLine("    Boolean Type: " + booleanTypeStr);
		lw->WriteLine("    Number of Target Bodies: " + std::to_string(targetBodies.size()));
		// 输出目标体的标签
		for (size_t i = 0; i < targetBodies.size(); ++i) {
			NXOpen::Body* body = targetBodies[i];
			lw->WriteLine("    Target Body " + std::to_string(i + 1) + ": JournalIdentifier = " + body->JournalIdentifier());
		}

		lw->WriteLine("  Has Offset: " + std::string(hasOffset ? "Yes" : "No"));
		if (hasOffset) {
			lw->WriteLine("    Start Offset: " + std::to_string(startOffset));
			lw->WriteLine("    End Offset: " + std::to_string(endOffset));
		}
		lw->WriteLine("  Has Draft: " + std::string(hasDraft ? "Yes" : "No"));
		if (hasDraft) {
			lw->WriteLine("    Draft Angle: " + std::to_string(draftAngle));
		}
		
		// 释放资源
		extrudeBuilder->Destroy();
	}
	else {
		lw->WriteLine("Failed to cast feature to Extrude.");
	}
}

void MyClass::extractRevolveData(NXOpen::Features::Feature* feature) {
	// 将特征转换为旋转特征
	NXOpen::Features::Revolve* revolveFeature = dynamic_cast<NXOpen::Features::Revolve*>(feature);
	if (revolveFeature) {
		// 创建旋转特征的构建器
		NXOpen::Features::RevolveBuilder* revolveBuilder = workPart->Features()->CreateRevolveBuilder(revolveFeature);

		//// 获取旋转轴
		NXOpen::Point* axisPointObj = revolveBuilder->Axis()->Point();
		NXOpen::Point3d axisOriginObj = revolveBuilder->Axis()->Origin();
		NXOpen::Direction* axisDirection = revolveBuilder->Axis()->Direction();
		NXOpen::Vector3d axisDirVector = axisDirection->Vector();

		// 获取旋转角度
		NXOpen::Expression* startExpression = revolveBuilder->Limits()->StartExtend()->Value();
		NXOpen::Expression* endExpression = revolveBuilder->Limits()->EndExtend()->Value();
		double startAngle = startExpression->Value();
		double endAngle = endExpression->Value();

		// 获取角度类型
		NXOpen::GeometricUtilities::Extend::ExtendType startType = revolveBuilder->Limits()->StartExtend()->TrimType();
		NXOpen::GeometricUtilities::Extend::ExtendType endType = revolveBuilder->Limits()->EndExtend()->TrimType();
		std::string startTypeStr = getExtendTypeAsString(startType);
		std::string endTypeStr = getExtendTypeAsString(endType);

		// 布尔操作类型
		NXOpen::GeometricUtilities::BooleanOperation::BooleanType booleanType = revolveBuilder->BooleanOperation()->Type();
		std::string booleanTypeStr;
		switch (int(booleanType)) {
		case 0: booleanTypeStr = "None"; break;  // 无
		case 1: booleanTypeStr = "Unite"; break;   // 合并
		case 2: booleanTypeStr = "Subtract"; break; // 减去
		case 3: booleanTypeStr = "Intersect"; break; // 相交
		default: booleanTypeStr = "Unknown"; break;
		}
		// 获取目标体
		std::vector<NXOpen::Body*> targetBodies = revolveBuilder->BooleanOperation()->GetTargetBodies();
		
		// 获取偏置信息
		bool hasOffset = revolveBuilder->Offset()->Option();
		double startOffset = 0.0, endOffset = 0.0;
		if (hasOffset) {
			startOffset = revolveBuilder->Offset()->StartOffset()->Value();
			endOffset = revolveBuilder->Offset()->EndOffset()->Value();
		}

		// 截面数据
		NXOpen::Section* section = revolveBuilder->Section();
		std::vector<NXOpen::SectionData*> sectionData;
		section->GetSectionData(sectionData);


		// 输出旋转特征数据
		lw->WriteLine("  Revolve Feature Data:");
		// 输出截面数据（与拉伸特征相同的逻辑）
		lw->WriteLine("    Revolve Section Data:");
		for (size_t i = 0; i < sectionData.size(); ++i) {
			NXOpen::SectionData* data = sectionData[i];
			lw->WriteLine("      Section Data " + std::to_string(i + 1) + ":");

			//// 获取选择意图规则
			//std::vector<NXOpen::SelectionIntentRule*> rules;
			//data->GetRules(rules);
			//for (size_t j = 0; j < rules.size(); ++j) {
			//	NXOpen::SelectionIntentRule::RuleType ruleType = rules[j]->Type();
			//	lw->WriteLine("    Rule " + std::to_string(j + 1) + ": " +
			//		getRuleTypeAsString(ruleType));
			//}

			// 获取截面元素
			std::vector<NXOpen::SectionElementData*> sectionElementsData;
			data->GetSectionElementsData(sectionElementsData);
			for (size_t j = 0; j < sectionElementsData.size(); ++j) {
				NXOpen::SectionElementData* elementData = sectionElementsData[j];
				lw->WriteLine("      Section Element Data " + std::to_string(j + 1) + ":");

				NXOpen::DisplayableObject* sectionElement;
				NXOpen::DisplayableObject* startConnectorElement;
				NXOpen::Point3d startPoint;
				NXOpen::DisplayableObject* endConnectorElement;
				NXOpen::Point3d endPoint;

				elementData->GetSectionElementData1(&sectionElement, &startConnectorElement,
					&startPoint, &endConnectorElement, &endPoint);

				if (sectionElement) {
					lw->WriteLine("        Section Element: " + sectionElement->Name());
				}
				lw->WriteLine("        Start Point: (" +
					std::to_string(startPoint.X) + ", " +
					std::to_string(startPoint.Y) + ", " +
					std::to_string(startPoint.Z) + ")");
				lw->WriteLine("        End Point: (" +
					std::to_string(endPoint.X) + ", " +
					std::to_string(endPoint.Y) + ", " +
					std::to_string(endPoint.Z) + ")");
			}
		}

		if (!axisPointObj) {
			lw->WriteLine("    Failed to get Point from Axis.");
			lw->WriteLine("    Axis Origin: (" +
				std::to_string(axisOriginObj.X) + ", " +
				std::to_string(axisOriginObj.Y) + ", " +
				std::to_string(axisOriginObj.Z) + ")");
		}
		else {
			NXOpen::Point3d axisPoint = axisPointObj->Coordinates();
			lw->WriteLine("    Axis Point: (" +
				std::to_string(axisPoint.X) + ", " +
				std::to_string(axisPoint.Y) + ", " +
				std::to_string(axisPoint.Z) + ")");
		}
		
		lw->WriteLine("    Axis Direction: (" +
			std::to_string(axisDirVector.X) + ", " +
			std::to_string(axisDirVector.Y) + ", " +
			std::to_string(axisDirVector.Z) + ")");
		
		lw->WriteLine("    Start Angle: " + std::to_string(startAngle) +
			" (" + startTypeStr + ")");
		lw->WriteLine("    End Angle: " + std::to_string(endAngle) +
			" (" + endTypeStr + ")");
		// 输出布尔操作信息
		lw->WriteLine("    Boolean Operation Data:");
		lw->WriteLine("      Boolean Type: " + booleanTypeStr);
		lw->WriteLine("      Number of Target Bodies: " + std::to_string(targetBodies.size()));
		// 输出目标体的标签
		for (size_t i = 0; i < targetBodies.size(); ++i) {
			NXOpen::Body* body = targetBodies[i];
			lw->WriteLine("      Target Body " + std::to_string(i + 1) + ": JournalIdentifier = " + body->JournalIdentifier());
		}
		lw->WriteLine("    Has Offset: " + std::string(hasOffset ? "Yes" : "No"));
		if (hasOffset) {
			lw->WriteLine("    Start Offset: " + std::to_string(startOffset));
			lw->WriteLine("    End Offset: " + std::to_string(endOffset));
		}
		

		// 释放资源
		revolveBuilder->Destroy();
	}
	else {
		lw->WriteLine("  Failed to cast feature to Revolve.");
	}
}


void MyClass::extractChamferData(NXOpen::Features::Feature* feature) {
	// 将特征转换为倒斜角特征
	NXOpen::Features::Chamfer* chamferFeature = dynamic_cast<NXOpen::Features::Chamfer*>(feature);
	if (chamferFeature) {
		// 创建倒斜角构建器
		NXOpen::Features::ChamferBuilder* chamferBuilder = workPart->Features()->CreateChamferBuilder(chamferFeature);

		// 1. 获取倒角类型
		string offset1, offset2, angle;
		bool reverseOffset;
		NXOpen::Features::ChamferBuilder::ChamferOption chamferType = chamferBuilder->Option();
		NXOpen::Features::ChamferBuilder::OffsetMethod  offsetMethod = chamferBuilder->Method();
		
		
		// 2. 获取倒角边列表
		std::vector<NXOpen::Edge*> chamferEdges;
		tag_t chamferEdgeTag;
		NXOpen::ScCollector* edgeCollector = chamferBuilder->SmartCollector();
		if (edgeCollector) {
			std::vector<NXOpen::TaggedObject*> edges = edgeCollector->GetObjects();
			for (auto obj : edges) {
				if (NXOpen::Edge* edge = dynamic_cast<NXOpen::Edge*>(obj)) {
					chamferEdgeTag = edge->Tag();
					chamferEdges.push_back(edge);
					
				}
			}
		}

		//std::vector<NXOpen::Features::Feature *> parentsFeatures = feature->GetParents();
		////lw->WriteLine("parentsFeatures size:" + std::to_string(parentsFeatures.size()));
		//std::vector<NXOpen::Edge*> parentEdges;
		//NXOpen::Features::Cylinder* cylinderFeature = dynamic_cast<NXOpen::Features::Cylinder*>(parentsFeatures[0]);
		//if (cylinderFeature) {
		//	// 创建构建器
		//	NXOpen::Features::CylinderBuilder* cylinderBuilder = workPart->Features()->CreateCylinderBuilder(cylinderFeature);
		//	//lw->WriteLine("Diameter " + cylinderBuilder->Diameter());
		//	parentEdges = cylinderFeature->GetEdges();
		//}

		//for (auto obj : parentEdges) {
		//	//lw->WriteLine("	Length" + std::to_string(obj->GetLength()));
		//	if (obj->Tag() == chamferEdgeTag){
		//		
		//		//chamferEdges.push_back(obj);
		//	}
		//}

		// 输出倒角数据
		lw->WriteLine("	Chamfer Feature Data:");

		// 输出边信息
		lw->WriteLine("  Chamfer Edges (" + std::to_string(chamferEdges.size()) + "):");
		for (size_t i = 0; i < chamferEdges.size(); ++i) {
			lw->WriteLine("    Edge " + std::to_string(i + 1));

			lw->WriteLine("      JournalIdentifier: " + chamferEdges[i]->JournalIdentifier());
			//lw->WriteLine("      Body Tag: " + chamferEdges[i]->GetBody()->JournalIdentifier());在抑制的对象上禁止该操作
			lw->WriteLine("      IsReference: " + std::string(chamferEdges[i]->IsReference() ? "Yes" : "No"));
		}


		// 输出参数值
		std::string typeStr,methodStr;
		switch (chamferType) {
		case NXOpen::Features::ChamferBuilder::ChamferOptionSymmetricOffsets:
			typeStr = "Symmetric offsets";
			offset1 = chamferBuilder->FirstOffset().GetLocaleText();
			lw->WriteLine("  Chamfer Type: " + typeStr);
			lw->WriteLine("  Symmetric Offset: " + offset1);
			break;
		case NXOpen::Features::ChamferBuilder::ChamferOptionTwoOffsets:
			typeStr = "Two offset distances";
			offset1 = chamferBuilder->FirstOffset().GetLocaleText();
			offset2 = chamferBuilder->SecondOffset().GetLocaleText();
			reverseOffset = chamferBuilder->ReverseOffsets();
			lw->WriteLine("  Chamfer Type: " + typeStr);
			lw->WriteLine("  First Offset: " + offset1);
			lw->WriteLine("  Second Offset: " + offset2);
			lw->WriteLine("  Offset Reverse Status : " + std::to_string(reverseOffset));
			break;
		case NXOpen::Features::ChamferBuilder::ChamferOptionOffsetAndAngle:
			typeStr = "Offset distance and angle";
			offset1 = chamferBuilder->FirstOffset().GetLocaleText();
			angle = chamferBuilder->Angle().GetLocaleText();
			reverseOffset = chamferBuilder->ReverseOffsets();
			lw->WriteLine("  Chamfer Type: " + typeStr);
			lw->WriteLine("  Offset: " + offset1);
			lw->WriteLine("  Angle: " + angle + " degrees");
			lw->WriteLine("  Offset Reverse Status : " + std::to_string(reverseOffset));
			break;
		default:
			typeStr = "Unknown Type";
		}
		switch (offsetMethod) {
		case NXOpen::Features::ChamferBuilder::OffsetMethodEdgesAlongFaces:
			methodStr = "Offset edges along faces";
			lw->WriteLine("  Offset Method: " + methodStr);
			break;
		case NXOpen::Features::ChamferBuilder::OffsetMethodFacesAndTrim:
			methodStr = "Offset faces and trim";
			lw->WriteLine("  Offset Method: " + methodStr);
			break;
		default:
			typeStr = "Unknown Type";
		}
		lw->WriteLine("  Tolerance : " + std::to_string(chamferBuilder->Tolerance()));


		// 释放资源
		chamferBuilder->Destroy();
	}
	else {
		lw->WriteLine("Failed to cast feature to Chamfer.");
	}
}

// 孔特征数据提取函数
void MyClass::extractHoleData(NXOpen::Features::Feature* feature) {
	// 将特征转换为孔特征
	NXOpen::Features::HolePackage* holeFeature = dynamic_cast<NXOpen::Features::HolePackage*>(feature);
	if (holeFeature) {
		// 创建孔特征构建器
		NXOpen::Features::HolePackageBuilder* holeBuilder = workPart->Features()->CreateHolePackageBuilder(holeFeature);

		// 提取孔特征的基本信息
		lw->WriteLine("Hole Feature Data:");

		NXOpen::Features::HolePackageBuilder::Types holeType = holeBuilder->Type();
		NXOpen::Features::HolePackageBuilder::HoleForms holeForm;
		std::string typeStr ;
		switch (holeType) {
		case NXOpen::Features::HolePackageBuilder::TypesGeneralHole:
			typeStr = "General hole";
			holeForm = holeBuilder->GeneralHoleForm();
			break;
		case NXOpen::Features::HolePackageBuilder::TypesDrillSizeHole:
			typeStr = "Drill Size hole";
			break;
		case NXOpen::Features::HolePackageBuilder::TypesScrewClearanceHole:
			typeStr = "Screw clearance hole";
			holeForm = holeBuilder->ScrewClearanceHoleForm();
			break;
		case NXOpen::Features::HolePackageBuilder::TypesThreadedHole:
			typeStr = "Threaded hole";
			break;
		case NXOpen::Features::HolePackageBuilder::TypesHoleSeries:
			typeStr = "Hole series";
			break;
		default:
			typeStr = "Unknown hole type";
			break;
		}
		lw->WriteLine("  Hole type: " + typeStr);
		
		//// 提取孔的位置信息
		//NXOpen::Point3d holeLocation = holeBuilder->HoleLocation();
		//lw->WriteLine("  Hole Location: (" +
		//	std::to_string(holeLocation.X) + ", " +
		//	std::to_string(holeLocation.Y) + ", " +
		//	std::to_string(holeLocation.Z) + ")");

		//孔方向
		NXOpen::GeometricUtilities::ProjectionOptions* holeDirectionOption = holeBuilder->ProjectionDirection();
		NXOpen::GeometricUtilities::ProjectionOptions::DirectionType directionType = holeDirectionOption->ProjectDirectionMethod();
		string directionStr;
		NXOpen::Vector3d projectVector;
		switch (directionType) {
		case NXOpen::GeometricUtilities::ProjectionOptions::DirectionTypeFaceNormal:
			directionStr = "Face Normal";
			lw->WriteLine("  Direction type: " + directionStr);
			break;
		case NXOpen::GeometricUtilities::ProjectionOptions::DirectionTypeCrvPlaneNormal:
			directionStr = "Curve plane Normal";
			lw->WriteLine("  Direction type: " + directionStr);
			break;
		case NXOpen::GeometricUtilities::ProjectionOptions::DirectionTypeVector:
			directionStr = "Vector Constructor";
			lw->WriteLine("  Direction type: " + directionStr);
			projectVector = holeDirectionOption->ProjectVector()->Vector();
			lw->WriteLine("  Project vector: (" +
				std::to_string(projectVector.X) + ", " +
				std::to_string(projectVector.Y) + ", " +
				std::to_string(projectVector.Z) + ")");
			break;
		default:
			directionStr = "Unknown direction type";
			lw->WriteLine("  Direction type: " + directionStr);
			break;
		}

		std::string subtypeStr;
		// 提取孔的类型(仅常规孔和螺钉间隙孔有)
		if (typeStr == "General hole" || typeStr == "Screw clearance hole") {
			switch (holeForm) {
			case NXOpen::Features::HolePackageBuilder::HoleFormsSimple:
				subtypeStr = "Simple hole form";
				break;
			case NXOpen::Features::HolePackageBuilder::HoleFormsCounterbored:
				subtypeStr = "Counterbore hole form";  //沉头
				break;
			case NXOpen::Features::HolePackageBuilder::HoleFormsCountersink:
				subtypeStr = "Countersink hole form";  //埋头
				break;
			case NXOpen::Features::HolePackageBuilder::HoleFormsTapered:
				subtypeStr = "Tapered hole form";      //锥孔
				break;
			default:
				subtypeStr = "Unknown Hole form";
				break;
			}
			lw->WriteLine("  Hole form: " + subtypeStr);
		}
		
		// 提取孔的附加参数（针对不同类型的孔）
		//常规孔
		if (typeStr == "General hole") {
			if (subtypeStr == "Simple hole form") {
				lw->WriteLine("  Diameter: " + std::to_string(holeBuilder->GeneralSimpleHoleDiameter()->Value()));
			}
			else if (subtypeStr == "Counterbore hole form") {
				// 提取沉头孔的参数
				lw->WriteLine("  Counter bore Diameter: " + std::to_string(holeBuilder->GeneralCounterboreDiameter()->Value()));
				lw->WriteLine("  Counter bore Depth: " + std::to_string(holeBuilder->GeneralCounterboreDepth()->Value()));
				lw->WriteLine("  Diameter: " + std::to_string(holeBuilder->GeneralCounterboreHoleDiameter()->Value()));
			}
			else if (subtypeStr == "Countersink hole form") {
				// 提取埋头孔的参数
				lw->WriteLine("  Counter sink diameter: " + std::to_string(holeBuilder->GeneralCountersinkDiameter()->Value()));
				lw->WriteLine("  Counter sink angle: " + std::to_string(holeBuilder->GeneralCountersinkAngle()->Value()));
				lw->WriteLine("  Diameter: " + std::to_string(holeBuilder->GeneralCountersinkHoleDepth()->Value()));

			}
			else if (subtypeStr == "Tapered hole form") {
				// 提取锥孔的参数
				lw->WriteLine("  Diameter: " + std::to_string(holeBuilder->GeneralTaperedHoleDiameter()->Value()));
				lw->WriteLine("  Taper angle: " + std::to_string(holeBuilder->GeneralTaperAngle()->Value()));

			}

		}
		//钻形孔
		else if (typeStr == "Drill Size hole") {
			string drillSize = holeBuilder->DrillSize().GetLocaleText();
			string drillFitOption = holeBuilder->DrillSizeFitOption().GetLocaleText();
			lw->WriteLine("  Drill size: " + drillSize);
			lw->WriteLine("  Drill size fit option: " + drillFitOption);
			lw->WriteLine("  Diameter: " + std::to_string(holeBuilder->DrillSizeHoleDiameter()->Value()));

		}
		//螺钉间隙孔
		else if (typeStr == "Screw clearance hole") {
			string screwType = holeBuilder->ScrewType().GetLocaleText();
			
			string screwSize = holeBuilder->ScrewSize().GetLocaleText();
			string screwFitOption = holeBuilder->ScrewFitOption().GetLocaleText();
			lw->WriteLine("  Screw type: " + screwType);
			lw->WriteLine("  Screw size: " + screwSize);
			lw->WriteLine("  Screw fit option: " + screwFitOption);
			
			// 提取沉头孔的参数
			if (subtypeStr == "Counterbore hole form") {
				lw->WriteLine("  Counter bore Diameter: " + std::to_string(holeBuilder->ScrewClearanceCounterboreDiameter()->Value()));
				lw->WriteLine("  Counter bore Depth: " + std::to_string(holeBuilder->ScrewClearanceCounterboreDepth()->Value()));
			}
			else if (subtypeStr == "Countersink hole form") {
				// 提取埋头孔的参数
				lw->WriteLine("  Counter sink diameter: " + std::to_string(holeBuilder->ScrewClearanceCountersinkDiameter()->Value()));
				lw->WriteLine("  Counter sink angle: " + std::to_string(holeBuilder->ScrewClearanceCountersinkAngle()->Value()));
			}
			lw->WriteLine("  Diameter: " + std::to_string(holeBuilder->ScrewClearanceHoleDiameter()->Value()));
		}
		//螺纹钉
		else if (typeStr == "Threaded hole") {
			string threadSize = holeBuilder->ThreadSize().GetLocaleText();
			NXOpen::Features::HolePackageBuilder::ThreadLengthOptions threadLengthOption = holeBuilder->ThreadLengthOption();
			NXOpen::Features::HolePackageBuilder::ThreadRotationOptions threadRotationOptions = holeBuilder->ThreadRotation();
			string threadLengthStr, threadRotationStr;

			switch (threadLengthOption) {
			case NXOpen::Features::HolePackageBuilder::ThreadLengthOptionsDiameterx1:
				threadLengthStr = "1 * (tap drill diameter) ";
				break;
			case NXOpen::Features::HolePackageBuilder::ThreadLengthOptionsDiameterx15:
				threadLengthStr = "1.5 * (tap drill diameter)";
				break;
			case NXOpen::Features::HolePackageBuilder::ThreadLengthOptionsDiameterx20:
				threadLengthStr = "2 * (tap drill diameter) ";
				break;
			case NXOpen::Features::HolePackageBuilder::ThreadLengthOptionsDiameterx25:
				threadLengthStr = "2.5 * (tap drill diameter) ";
				break;
			case NXOpen::Features::HolePackageBuilder::ThreadLengthOptionsDiameterx30:
				threadLengthStr = "3 * (tap drill diameter)";
				break;
			case NXOpen::Features::HolePackageBuilder::ThreadLengthOptionsStandard:
				threadLengthStr = "Length is Standard";
				break;
			case NXOpen::Features::HolePackageBuilder::ThreadLengthOptionsCustom:
				threadLengthStr = "Length is custom";
				break;
			case NXOpen::Features::HolePackageBuilder::ThreadLengthOptionsFull:
				threadLengthStr = "Length is full hole depth";
				break;
			default:
				threadLengthStr = "Unknown";
				break;
			}

			switch (threadRotationOptions) {
			case NXOpen::Features::HolePackageBuilder::ThreadRotationOptionsRight:
				threadRotationStr = "Right rotation";
				break;
			case NXOpen::Features::HolePackageBuilder::ThreadRotationOptionsLeft:
				threadRotationStr = "Left rotation";  
				break;
			default:
				threadRotationStr = "Unknown rotation";
				break;
			}

			lw->WriteLine("  Thread size: " + threadSize);
			lw->WriteLine("  Tap drill diameter: " + std::to_string(holeBuilder->TapDrillDiameter()->Value()));
			lw->WriteLine("  Thread length option: " + threadLengthStr);
			lw->WriteLine("  Thread rotation option: " + threadRotationStr);
		}
		


		//深度限制
		NXOpen::Features::HolePackageBuilder::HoleDepthLimitOptions depthLimit = holeBuilder->HoleDepthLimitOption();
		string depthLimitStr;
		switch (depthLimit) {
			case NXOpen::Features::HolePackageBuilder::HoleDepthLimitOptionsValue:
				depthLimitStr = "Value limit options";
				break;
			case NXOpen::Features::HolePackageBuilder::HoleDepthLimitOptionsUntilSelected:
				depthLimitStr = "Until selected limit options";  
				break;
			case NXOpen::Features::HolePackageBuilder::HoleDepthLimitOptionsUntilNext:
				depthLimitStr = "Until next limit options";  
				break;
			case NXOpen::Features::HolePackageBuilder::HoleDepthLimitOptionsThroughBody:
				depthLimitStr = "Through body limit options";      
				break;
			default:
				depthLimitStr = "Unknown deoth limit options";
				break;
		}
		lw->WriteLine("  Depth limit: " + depthLimitStr);

		//深度限制为值时
		if (depthLimitStr == "Value limit options") {
			//常规孔
			if (typeStr == "General hole") {
				if (subtypeStr == "Simple hole form") {
					lw->WriteLine("  Depth: " + std::to_string(holeBuilder->GeneralSimpleHoleDepth()->Value()));
				}
				else if (subtypeStr == "Counterbore hole form") {
					lw->WriteLine("  Depth: " + std::to_string(holeBuilder->GeneralSimpleHoleDepth()->Value()));
				}
				else if (subtypeStr == "Countersink hole form") {
					lw->WriteLine("  Depth: " + std::to_string(holeBuilder->GeneralCountersinkHoleDepth()->Value()));
				}
				else if (subtypeStr == "Tapered hole form") {
					lw->WriteLine("  Depth: " + std::to_string(holeBuilder->GeneralTaperedHoleDepth()->Value()));
				}

				lw->WriteLine("  Tip angle: " + std::to_string(holeBuilder->GeneralTipAngle()->Value()));
			}
			//钻形孔
			else if (typeStr == "Drill Size hole") {
				lw->WriteLine("  Depth: " + std::to_string(holeBuilder->DrillSizeHoleDepth()->Value()));
				lw->WriteLine("  Tip angle: " + std::to_string(holeBuilder->DrillSizeTipAngle()->Value()));
				string drillSizeStandard = holeBuilder->DrillSizeStandard().GetLocaleText();
				lw->WriteLine("  Drill size standard: " + drillSizeStandard);
			}
			//螺钉间隙孔
			else if (typeStr == "Screw clearance hole") {
				lw->WriteLine("  Depth: " + std::to_string(holeBuilder->ScrewClearanceHoleDepth()->Value()));

				lw->WriteLine("  Tip angle: " + std::to_string(holeBuilder->ScrewClearanceTipAngle()->Value()));
				string screwStandard = holeBuilder->ScrewStandard().GetLocaleText();
				lw->WriteLine("  Screw standard: " + screwStandard);
			}
			else if (typeStr == "Threaded hole") {
				lw->WriteLine("  Depth: " + std::to_string(holeBuilder->ThreadedHoleDepth()->Value()));

				lw->WriteLine("  Tip angle: " + std::to_string(holeBuilder->ThreadedTipAngle()->Value()));
				string threadStandard = holeBuilder->ThreadStandard().GetLocaleText();
				lw->WriteLine("  Thread standard: " + threadStandard);
			}
		}

		// 获取布尔操作类型
		NXOpen::GeometricUtilities::BooleanOperation::BooleanType booleanType;
		NXOpen::Body* targetBody;
		holeBuilder->BooleanOperation()->GetBooleanOperationAndBody(&booleanType, &targetBody);
		std::string booleanTypeStr;
		switch (int(booleanType)) {
		case 0: booleanTypeStr = "None"; break;  // 无
		case 1: booleanTypeStr = "Unite"; break;   // 合并
		case 2: booleanTypeStr = "Subtract"; break; // 减去
		case 3: booleanTypeStr = "Intersect"; break; // 相交
		default: booleanTypeStr = "Unknown"; break;
		}
		// 输出布尔操作信息
		lw->WriteLine("  Boolean Operation Data:");
		lw->WriteLine("    Boolean Type: " + booleanTypeStr);
		lw->WriteLine("    Target Body: JournalIdentifier = " + targetBody->JournalIdentifier());
		lw->WriteLine("  Tolerance : " + std::to_string(holeBuilder->Tolerance()));
		// 释放资源
		holeBuilder->Destroy();
	}
	else {
		lw->WriteLine("Failed to cast feature to HoleFeature.");
	}
}

//------------------------------------------------------------------------------
// Do something
//------------------------------------------------------------------------------
void MyClass::do_it()
{

	// TODO: add your code here
	const char* filePath = "C:/Users/kamijou/Desktop/output_log.txt"; // 文件路径
	lw->SelectDevice(
		NXOpen::ListingWindow::DeviceTypeFileAndWindow, // 设备类型：窗口
		filePath                              // 文件路径
	);
	lw->Open();//打开列表窗口，用于显示消息和调试信息

	NXOpen::Features::FeatureCollection::iterator i;
	NXOpen::Features::Feature* fcList;
		
	for (i = workPart->Features()->begin(); i != workPart->Features()->end(); i++)

	{
		fcList = (*i);
			
		extractFeatureData(fcList);
	}
	//NXOpen::UI::GetUI()->NXMessageBox()->Show("", NXMessageBox::DialogType::DialogTypeInformation, "OK!");
	//  恢复输出到窗口
	lw->SelectDevice(NXOpen::ListingWindow::DeviceTypeWindow, nullptr);

}

//------------------------------------------------------------------------------
// Entry point(s) for unmanaged internal NXOpen C/C++ programs
//------------------------------------------------------------------------------
//  NX Startup
extern "C" DllExport void ufsta( char *param, int *returnCode, int rlen )
{
    try
    {
		// Create NXOpen C++ class instance
		MyClass *theMyClass;
		theMyClass = new MyClass();
		theMyClass->do_it();
		delete theMyClass;
	}
    catch (const NXException& e1)
    {
		UI::GetUI()->NXMessageBox()->Show("NXException", NXOpen::NXMessageBox::DialogTypeError, e1.Message());
    }
	catch (const exception& e2)
    {
		UI::GetUI()->NXMessageBox()->Show("Exception", NXOpen::NXMessageBox::DialogTypeError, e2.what());
    }
	catch (...)
    {
		UI::GetUI()->NXMessageBox()->Show("Exception", NXOpen::NXMessageBox::DialogTypeError, "Unknown Exception.");
    }
}
//  Explicit Execution
extern "C" DllExport void ufusr( char *parm, int *returnCode, int rlen )
{
    try
    {
		// Create NXOpen C++ class instance
		MyClass *theMyClass;
		theMyClass = new MyClass();
		theMyClass->do_it();
		delete theMyClass;
	}
    catch (const NXException& e1)
    {
		UI::GetUI()->NXMessageBox()->Show("NXException", NXOpen::NXMessageBox::DialogTypeError, e1.Message());
    }
	catch (const exception& e2)
    {
		UI::GetUI()->NXMessageBox()->Show("Exception", NXOpen::NXMessageBox::DialogTypeError, e2.what());
    }
	catch (...)
    {
		UI::GetUI()->NXMessageBox()->Show("Exception", NXOpen::NXMessageBox::DialogTypeError, "Unknown Exception.");
    }
}


//------------------------------------------------------------------------------
// Unload Handler
//------------------------------------------------------------------------------
extern "C" DllExport int ufusr_ask_unload()
{
	return (int)NXOpen::Session::LibraryUnloadOptionImmediately;
}


