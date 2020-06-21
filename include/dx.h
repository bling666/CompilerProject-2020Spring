#include "IR.h"
#include "IRMutator.h"
#include "IRVisitor.h"
#include "type.h"
#include "token.h"
#include<string>
#include<algorithm>
#include<map>
#include<vector>
//using namespace Boost::Internal;
namespace Boost{
	namespace Internal{
        namespace dx{

//Type index_type = Type::int_scalar(32);
//Type data_type = Type::float_scalar(32);

void P();
void S();
void LHS();
void RHS();
void RHS1();
void term();
void TSRef() ;
void CList();
void CList1();
void AList();
void AList1();
void IdExpr() ;
void IdExpr1() ;
void Const() ;
std::string te(std::string kernel,std::vector<std::string> & in,std::vector<std::string> & out, std::vector<std::string> & grad_to);
        }
	}
}

