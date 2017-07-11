#include <cstddef>
#include <cassert>
#include <string>
#include <vector>

#include "../AST.h"

using namespace horseIR::ast ;

FunctionType::FunctionType(ASTNode* parent, HorseIRParser::TypeCaseFuncContext* cst, ASTNode::MemManagerType& mem)
    : Type(parent, cst, mem, Type::TypeClass::Function, ASTNode::ASTNodeClass::FunctionType)
{
    assert(cst != nullptr) ;
    
    HorseIRParser::TypeFunc0Context* type0 = nullptr ;
    HorseIRParser::TypeFunc1Context* type1 = nullptr ;
    HorseIRParser::TypeFunc2Context* type2 = nullptr ;
    HorseIRParser::TypeFunc3Context* type3 = nullptr ;

    auto funcCST = cst->typeFunc() ;
    if ((type0 = dynamic_cast<decltype(type0)>(funcCST)) != nullptr) {
        flexible = false ;
        returnType = Type::makeTypeASTNode(this, type0->type(), mem) ;
    } else if ((type1 = dynamic_cast<decltype(type1)>(funcCST)) != nullptr) {
        flexible = true ;
        returnType = Type::makeTypeASTNode(this, type1->type(), mem) ;
    } else if ((type2 = dynamic_cast<decltype(type2)>(funcCST)) != nullptr) {
        flexible = false ;
        std::vector<HorseIRParser::TypeContext*> types(std::move(type2->type())) ;
        for (auto ptr = types.cbegin(); ptr != types.cend(); ++ptr) {
            auto param = Type::makeTypeASTNode(this, *ptr, mem) ;
            if (ptr + 1 != types.cend()) {
                parameterTypes.push_back(param) ;
            } else {
                returnType = param ;
            }
        }
    } else if ((type3 = dynamic_cast<decltype(type3)>(funcCST)) != nullptr) {
        flexible = true ;
        std::vector<HorseIRParser::TypeContext*> types(std::move(type3->type())) ;
        for (auto ptr = types.cbegin(); ptr != types.cend(); ++ptr) {
            auto param = Type::makeTypeASTNode(this, *ptr, mem) ;
            if (ptr + 1 != types.cend()) {
                parameterTypes.push_back(param) ;
            } else {
                returnType = param ;
            }
        }
    } else {
        assert(false) ;
    }
}

FunctionType::FunctionType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::Function, ASTNode::ASTNodeClass::FunctionType),
      flexible(false),
      returnType(nullptr)
{}

bool FunctionType::isGeneralizationOf(const Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Function) return false ;
    auto functionType = static_cast<const FunctionType*>(type) ;
    
    bool paramGeneralization = true ;
    if (flexible) {
        if (parameterTypes.size() > functionType->parameterTypes.size()) {
            paramGeneralization = false ;
        }
    } else {
        if (parameterTypes.size() != functionType->parameterTypes.size()) {
            paramGeneralization = false ;
        }
    }
    if (!paramGeneralization) return false ;

    for (std::size_t ptr = 0; ptr < parameterTypes.size(); ++ptr) {
        const auto myParam = parameterTypes.at(ptr) ;
        const auto targetParam = parameterTypes.at(ptr) ;
        if (!myParam->isGeneralizationOf(targetParam)) return false ;
    }

    return returnType->isGeneralizationOf(functionType->returnType) ;
}

std::size_t FunctionType::getNumNodesRecursively() const
{
    std::size_t count = 1 ;
    count += (returnType == nullptr)? 0: returnType->getNumNodesRecursively() ;
    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        if (*ptr != nullptr) {
            count += (*ptr)->getNumNodesRecursively() ;
        }
    }
    return count ;
}

std::vector<ASTNode*> FunctionType::getChildren() const
{
    std::vector<ASTNode*> retVector ;
    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        if (*ptr != nullptr) {
            ASTNode* node_ptr = *ptr ;
            retVector.push_back(node_ptr) ;
        }
    }
    if (returnType != nullptr) {
        ASTNode* ret_ptr = returnType ;
        retVector.push_back(ret_ptr) ;
    }
    return retVector ;
}

std::string FunctionType::toString() const
{
    std::ostringstream ostream ;
    ostream << "func<" ;
    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        Type* paramType = *ptr ;
        ostream << paramType->toString()
                << ((ptr + 1 == parameterTypes.cend())? "" : ", ") ;
    }
    if (parameterTypes.size() != 0) ostream << ", " ;
    ostream << (flexible? "..." : "")
            << " :"
            << ((returnType!= nullptr)? returnType->toString() : "nullptr")
            << ">" ;
    return ostream.str() ;
}

std::string FunctionType::toTreeString() const
{
    std::ostringstream ostream ;
    ostream << std::boolalpha
            << "(FunctionType" ;
    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        Type* paramType = *ptr ;
        ostream << " "
                << paramType->toTreeString() ;
    }
    ostream << " "
            << returnType->toTreeString()
            << " "
            << flexible
            << ")" ;
    return ostream.str() ;
}

std::size_t FunctionType::getMinNumParameters() const
{
    return parameterTypes.size() ;
}

bool FunctionType::isFlexible() const
{
    return flexible ;
}

Type* FunctionType::getReturnType() const
{
    return returnType ;
}

std::vector<Type*> FunctionType::getParameterTypes() const
{
    return parameterTypes ;
}

Type* FunctionType::getParameterTypeAt(std::size_t pos) const
{
    return parameterTypes.at(pos) ;
}

FunctionType& FunctionType::addParameterType(horseIR::ast::Type *type)
{
    assert(type != nullptr) ;
    
    parameterTypes.push_back(type) ;

    return *this ;
}

FunctionType& FunctionType::setParameterTypeAt(std::size_t pos, horseIR::ast::Type *type)
{
    assert(type != nullptr) ;
    assert(pos < parameterTypes.size()) ;

    parameterTypes[pos] = type ;

    return *this ;
}

FunctionType& FunctionType::setFlexible(bool f)
{
    flexible = f ;
    return *this ;
}

FunctionType& FunctionType::setReturnType(horseIR::ast::Type *type)
{
    assert(type != nullptr) ;

    returnType = type ;

    return *this ;
}

FunctionType& FunctionType::truncateNumParameter(std::size_t num)
{
    if (num >= parameterTypes.size()) return *this ; /* NO-OP */

    std::vector<Type*> org_parameterTypes = parameterTypes ;
    parameterTypes.clear() ;
    for (std::size_t ptr = 0; ptr < num; ++ptr) {
        parameterTypes.push_back(org_parameterTypes[ptr]) ;
    }

    return *this ;
}
