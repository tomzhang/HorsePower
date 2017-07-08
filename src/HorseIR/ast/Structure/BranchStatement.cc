#include <vector>
#include <cassert>
#include <string>
#include <utility>
#include <sstream>
#include "../grammar/HorseIRParser.h"

#include "../Structure.h"

using namespace horseIR::ast ;

BranchStatement::BranchStatement(HorseIRParser::StmtCoreContext *cst, ASTNode::MemManagerType &mem)
    : Statement(cst, mem, ASTNode::ASTNodeClass::BranchStatement, StatementClass::Branch)
{
    assert(cst != nullptr) ;
    assert(cst->statementCore() != nullptr) ;
    auto gotoContext = dynamic_cast<HorseIRParser::StmtGotoContext*>(cst->statementCore()) ;
    assert(gotoContext != nullptr) ;

    targetLabelName = gotoContext->label()->getText() ;
    if (gotoContext->name() != nullptr) {
        Identifier* identifier = new Identifier(gotoContext->name(), mem) ;
        checkCondition = std::make_pair(true, identifier) ;
    } else {
        checkCondition = std::make_pair(false, nullptr) ;
    }
}

BranchStatement::BranchStatement(ASTNode::MemManagerType &mem)
    : Statement(mem, ASTNode::ASTNodeClass::BranchStatement, StatementClass::Branch),
      targetLabelName{""},
      checkCondition{std::make_pair(false, nullptr)}
{}

std::size_t BranchStatement::getNumNodesRecursively() const
{
    return (checkCondition.first)? checkCondition.second->getNumNodesRecursively() + 1 : 1 ;
}

std::vector<ASTNode*> BranchStatement::getChildren() const
{
    return (checkCondition.first)? std::vector<ASTNode*> {checkCondition.second} : std::vector<ASTNode*> {} ;
}

std::string BranchStatement::toString() const
{
    std::ostringstream stream ;
    stream << "goto " << targetLabelName ;
    if (checkCondition.first) {
        stream << ' ' << checkCondition.second->toString() ;
    }
    stream << " ;" ;
    return stream.str() ;
}

std::string BranchStatement::toTreeString() const
{
    return "(BranchStatement)" ;
}