#include "IR.h"
#include "IRMutator.h"
#include "IRVisitor.h"
#include "type.h"
#include "token.h"
#include<string>
#include<algorithm>
#include<map>
#include<vector>
#include"json.h"

//using namespace Boost::Internal;
namespace Boost{
	namespace Internal{

//Type index_type = Type::int_scalar(32);
//Type data_type = Type::float_scalar(32);

std::vector<Stmt> P(Type index_type, Type data_type);
std::vector<Stmt> S(Type index_type, Type data_type);
Expr LHS(Type index_type, Type data_type);
Expr RHS(Type index_type, Type data_type);
Expr RHS1(Type index_type, Type data_type, Expr expr_factor);
Expr term(Type index_type, Type data_type);
Expr TSRef(Type index_type, Type data_type) ;
std::vector<size_t> CList(Type index_type, Type data_type);
std::vector<size_t> CList1(Type index_type, Type data_type);
std::vector<Expr> AList(Type index_type, Type data_type, std::vector<size_t> dom);
std::vector<Expr> AList1(Type index_type, Type data_type, std::vector<size_t> dom);
Expr IdExpr(Type index_type, Type data_type, int domsize) ;
Expr IdExpr1(Type index_type, Type data_type, int domsize, std::string id,Expr tok) ;
Expr Const(Type index_type, Type data_type) ;

std::string print_temp();
std::string compile_project1(json json_config);

	}
}

