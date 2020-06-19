#include "IR.h"
#include "IRMutator.h"
#include "IRVisitor.h"
#include "IRPrinter.h"
#include "type.h"
#include "token.h"
#include "kernel.h"
#include "lex.h"
#include<string>
#include<algorithm>
#include<map>
#include<vector>
#include<iostream>
#include<cstring>


namespace Boost{
	namespace Internal{

Type index_type = Type::int_scalar(32);
Type data_type ;
 

		class Myclass {   //Index's dom and name
		public:
			size_t dom;
			std::string name;
		};
		std::vector<std::string> all_temp_names;
		std::vector< std::vector<size_t> > all_temp_size;
		std::vector<Expr> all_temp_vars;
		size_t temp_count ;



		int ID = 0;   //define ID 0

	//	std::string s ="A<16, 32>[i, j] = 2; ";  //
	//	std::vector<Token> tokens =  mylex(s);   //get tokes from token parse

		std::vector<size_t> left_size;  //store size of array temp
		std::vector<Expr> left_index;
		std::vector<std::string> left_name;
		std::vector<Expr> temp_vars;
		
		std::vector<Expr> total_index;
		std::vector<size_t> total_size;  //store all index-name-size
		std::vector<std::string> total_name;
		
		int wrap_layer;

		std::vector<Expr> all_vars;      //need to judge if exist!!
		std::vector<std::string> all_vars_name;
		std::vector<Expr> output_vars;
		std::vector<Expr> input_vars;

		std::vector<Expr> total_cond;
		bool flag = false;

		std::vector<Stmt> extra_stmt;
		std::vector<std::vector<std::string> > extra_index;
        json res;
        std::string s;
        std::vector<string> ins;
        std::vector<string> outs;
        
        std::vector<Token> tokens;
		int token_index;  //index for tokens
		Token look; //next token
		void add_index(Myclass a,Expr exp) {
			for (int i = 0; i < total_size.size(); i++) {
				if (total_name[i] == a.name) {
					if (total_size[i] > a.dom) {
						total_size[i] = a.dom;
						total_index[i] = exp;
					}
					return;
				}
			}
			total_index.push_back(exp);
			total_name.push_back(a.name);
			total_size.push_back(a.dom);

		}
		void add_var(std::string name, Expr expr)
		{
			for(int i=0 ;i< all_vars.size();i++)
			{
				if(name == all_vars_name[i])
					return ;
			}
			all_vars_name.push_back(name);
			all_vars.push_back(expr);
		}
		
		Token match(int k)
		{
			if(tokens[token_index].type!=k)
			{
				std::cout<<"match is wrong!";
			}
			else
			{
				if(tokens[token_index].type==0)
				std::cout<<"successfully match "<< tokens[token_index].stringValue <<'\n';
				else if(tokens[token_index].type==1)
				std::cout<<"successfully match "<< tokens[token_index].intValue <<'\n';
				else if(tokens[token_index].type==2)
				std::cout<<"successfully match "<< tokens[token_index].floatValue <<'\n';
				else
				std::cout<<"successfully match "<< (char)k <<'\n';
			}
			if(token_index == tokens.size() - 1)
				return look;
			if (k == 0 || k == 1 || k == 2)
			{
				Token tmp_token = tokens[token_index];
				token_index++;
				look = tokens[token_index];
				return tmp_token;
			}
			else
			{
				token_index++;
				look = tokens[token_index];
				return look;
			}
		}
		 std::string print_temp()
        {
        	std::ostringstream temp_oss;
            for(int i=0; i<all_temp_names.size(); i++)
            {
                if(data_type == Type::int_scalar(32))
                    temp_oss <<  "int " << all_temp_names[i];
                else if(data_type == Type::float_scalar(32))
                    temp_oss <<  "float " << all_temp_names[i];
                int dim = all_temp_size[i].size();
                for(int j = 0;j < dim; j++)
                    temp_oss << "[" << all_temp_size[i][j] << "]";
                temp_oss << ";\n";
            }
            return temp_oss.str();
            
        }

		//P ::= S*
		std::vector<Stmt> P(Type index_type, Type data_type)
		{
			std::vector<Stmt> stmts;
			temp_count = 1;

			while (look.type == ID)
			{
				total_size.clear();
				total_index.clear();
				total_name.clear();
				total_cond.clear();
				extra_stmt.clear();
				extra_index.clear();
				wrap_layer = 0;
				
				//����S���ս���Ĵ�������
				std::cout<<"enter S"<<'\n';
				std::vector<Stmt> sts = S(index_type, data_type);
				std::cout<<"leave S"<<'\n';

				//loopΪ����ѭ�����±꣬����ѭ������
				
				for(size_t i = 0;i < sts.size(); i++)	
					stmts.push_back(sts[i]);
				//ƥ��ĩβ�ֺ�
				match(';');

				//std::cout<<"P is success"<<'\n';
			}
			//Ϊÿ��������乹��һ��ѭ���壬��������ѭ�����vector
			return stmts;
		}

		//S ::= LHS = RHS
		std::vector<Stmt> S(Type index_type, Type data_type)
		{
			std::vector<Stmt> sts;
			std::cout<<"enter LHS"<<'\n';
			Expr expr_L = LHS(index_type, data_type);
			std::cout<<"leave LHS"<<'\n';

			left_size.clear();
			left_index.clear();
			left_name.clear();
			temp_vars.clear();
			for(int i=0;i<total_index.size();i++)
			{
				left_index.push_back(total_index[i]);
				left_size.push_back(total_size[i]);
				left_name.push_back(total_name[i]);
			}

			all_temp_size.push_back(left_size);
			std::string temp_name = "temp";
			temp_name += to_string(temp_count);
			all_temp_names.push_back(temp_name);
			temp_count++;

			Expr expr_temp = Var::make(data_type, temp_name, left_index, left_size);
			all_temp_vars.push_back(expr_temp);
			temp_vars.push_back(expr_temp);

			
			match('=');
			Expr expr_R = RHS(index_type, data_type);
/*
			
			for (int i = 0; i < total_index.size(); i++) 
			{
					loop.push_back(total_index[i]);
			}  */

			std::vector<Stmt> tmp_sts;
			for(size_t i=0;i<temp_vars.size();i++)
			{
				Stmt init_temp = Move::make(temp_vars[i], Expr(0), MoveType::MemToMem);
				tmp_sts.push_back(init_temp);
			}

			for(size_t i=0;i<extra_stmt.size();i++)
			{
				if(extra_index[i].size() == 0)
					tmp_sts.push_back(extra_stmt[i]);
				else
				{
					std::vector<Expr> iner_loop ;
					for(size_t j=0;j<extra_index[i].size();j++)
					{
						for(size_t k=0;k<total_index.size();k++)
						{
							if(total_name[k] == extra_index[i][j])
							{
								iner_loop.push_back(total_index[k]);
								break;
							}
						}
					}
					Stmt wrong_stmt = extra_stmt[i];
					Stmt if_stmt = extra_stmt[i];
					for(size_t j=0;j<total_cond.size();j++)
					{
						if_stmt = IfThenElse::make(total_cond[j], if_stmt, wrong_stmt);
					}
					Stmt iner_loop_nest = LoopNest::make(iner_loop, {if_stmt});
					tmp_sts.push_back(iner_loop_nest);
				}
			}


			
			/*
			Stmt stmt1 = Move::make(expr_temp, expr_R, MoveType::MemToMem);
			Stmt stm_wrong = Move::make(expr_temp, expr_R, MoveType::MemToMem);
			for(size_t i=0;i<total_cond.size();i++)
			{
				stmt1 = IfThenElse::make(total_cond[i], stmt1, stm_wrong);
			}*/
			
			Stmt stmt1 = Move::make(expr_temp, expr_R, MoveType::MemToMem);
			tmp_sts.push_back(stmt1);
			Stmt loop_nest1 = LoopNest::make(left_index, tmp_sts);        //loop:

			tmp_sts.clear();
			Stmt stmt2 = Move::make(expr_L, expr_temp, MoveType::MemToMem);
			tmp_sts.push_back(stmt2);
			Stmt loop_nest2 = LoopNest::make(left_index, tmp_sts); 

			sts.push_back(loop_nest1);
			sts.push_back(loop_nest2);

			//Stmt main_stmt = Move::make(expr_L, expr_R, MoveType::MemToMem);
			//sts.push_back(main_stmt);
			return sts;

		}
		//LHS ::= TRef
		Expr LHS(Type index_type, Type data_type)
		{
			std::cout<<"enter TSRef"<<'\n';
			return TSRef(index_type, data_type);
			std::cout<<"leave TSRef"<<'\n';
		}

		//RHS1
		Expr RHS1(Type index_type, Type data_type, Expr expr)
		{
			
			if (look.type == '+')
			{
				match('+');
				Expr expr_rhs = RHS(index_type, data_type);
				Expr ret = Binary::make(data_type, BinaryOpType::Add, expr, expr_rhs);
				return ret;
			}
			else if (look.type == '*')
			{
				match('*');
				Expr expr_rhs = term(index_type, data_type);
				Expr ret = Binary::make(data_type, BinaryOpType::Mul, expr, expr_rhs);
				return ret;
			}
			else if (look.type == '-')
			{
				match('-');
				Expr expr_rhs = RHS(index_type, data_type);
				Expr ret = Binary::make(data_type, BinaryOpType::Sub, expr, expr_rhs);
				return ret;
			}
			else if (look.type == '%')
			{
				match('%');
				Expr expr_rhs = term(index_type, data_type);
				Expr ret = Binary::make(data_type, BinaryOpType::Mod, expr, expr_rhs);
				return ret;
			}
			else if (look.type == '/')
			{
				match('/');
				Expr expr_rhs = term(index_type, data_type);
				Expr ret = Binary::make(data_type, BinaryOpType::Div, expr, expr_rhs);
				return ret;
			}
			else if (look.type == 3)
			{
				match(3);
				Expr expr_rhs = term(index_type, data_type);
				Expr ret = Binary::make(data_type, BinaryOpType::Div, expr, expr_rhs);
				return ret;
			}
			else
			{
				std::cout<<"something is wrong!\n";
				return expr;
			}
		}

		std::vector<std::string> temp_record;
		//RHS::= RHS + term | RHS - term | term
		Expr RHS(Type index_type, Type data_type)
		{
			wrap_layer++;
			if(wrap_layer ==1)
			{
				temp_record.clear();
			}
			Expr expr = term(index_type, data_type);
		if(wrap_layer == 1)
		{

			extra_index.push_back(temp_record);

			all_temp_size.push_back(left_size);
			std::string temp_name = "temp";
	 		temp_name+=to_string(temp_count);
			all_temp_names.push_back(temp_name);
			temp_count++;

			Expr expr_temp = Var::make(data_type, temp_name, left_index, left_size);
			all_temp_vars.push_back(expr_temp);
			temp_vars.push_back(expr_temp);

			Stmt stmt_1 = Move::make(expr_temp, 
				Binary::make(data_type, BinaryOpType::Add, expr_temp, expr), MoveType::MemToMem);

			extra_stmt.push_back(stmt_1);

			if(look.type == '+'||look.type == '-')
			{
				wrap_layer--;
				return RHS1(index_type, data_type, expr_temp);
			}
			wrap_layer--;
			return expr_temp;
		}
		else
		{

			if(look.type == '+'||look.type == '-')
			{
				wrap_layer--;
				return RHS1(index_type, data_type, expr);
			}
			wrap_layer--;
			return expr;
		}

		}

		//term::= term*factor | term/factor | term%factor | term//factor | factor
		//factor::= Const | (RHS) | TRef | SRef
		Expr term(Type index_type, Type data_type)
		{	Expr expr;

			if(look.type ==1 || look.type == 2)
			{
				 expr = Const(index_type, data_type);
				 if(look.type == '*'||look.type == '/'||look.type == '%'||look.type == '3')
				 	return RHS1(index_type, data_type, expr);
				 return expr;
			}
			else if(look.type == '(')
			{
				match('(');
				expr = RHS(index_type, data_type);
				match(')');
				if(look.type == '*'||look.type == '/'||look.type == '%'||look.type == '3')
				 	return RHS1(index_type, data_type, expr);
				 return expr;
			}
			else
			{
				expr =TSRef(index_type, data_type);
				if(look.type == '*'||look.type == '/'||look.type == '%'||look.type == '3')
				 	return RHS1(index_type, data_type, expr);
				 return expr;
			}

		}
/*
		
		//RHS ::= factor RHS1
		//RHS1 ::= +RHS RHS1 || *RHS RHS1 || -RHS RHS1 || %RHS RHS1 || //RHS RHS1 || /RHS RHS1 || epsilon
		//factor ::= TRef | SRef | Const | (RHS)
		

		Expr RHS(Type index_type, Type data_type)
		{
			
			Expr expr_factor = factor(index_type, data_type);
			Expr expr = RHS1(index_type, data_type, expr_factor);

			return expr;
		}

		//RHS1
		Expr RHS1(Type index_type, Type data_type, Expr expr_factor)
		{
			
			if (look.type == '+')
			{
				match('+');
				Expr expr_rhs = RHS(index_type, data_type);
				Expr expr = Binary::make(data_type, BinaryOpType::Add, expr_factor, expr_rhs);
				return RHS1(index_type, data_type, expr);
			}
			else if (look.type == '*')
			{
				match('*');
				Expr expr_rhs = RHS(index_type, data_type);
				Expr expr = Binary::make(data_type, BinaryOpType::Mul, expr_factor, expr_rhs);
				return RHS1(index_type, data_type, expr);
			}
			else if (look.type == '-')
			{
				match('-');
				Expr expr_rhs = RHS(index_type, data_type);
				Expr expr = Binary::make(data_type, BinaryOpType::Sub, expr_factor, expr_rhs);
				return RHS1(index_type, data_type, expr);
			}
			else if (look.type == '%')
			{
				match('%');
				Expr expr_rhs = RHS(index_type, data_type);
				Expr expr = Binary::make(data_type, BinaryOpType::Mod, expr_factor, expr_rhs);
				return RHS1(index_type, data_type, expr);
			}
			else if (look.type == '/')
			{
				match('/');
				Expr expr_rhs = RHS(index_type, data_type);
				Expr expr = Binary::make(data_type, BinaryOpType::Div, expr_factor, expr_rhs);
				return RHS1(index_type, data_type, expr);
			}
			else if (look.type == 3)
			{
				match(3);
				Expr expr_rhs = RHS(index_type, data_type);
				Expr expr = Binary::make(data_type, BinaryOpType::Div, expr_factor, expr_rhs);
				return RHS1(index_type, data_type, expr);
			}
			else
			{
				return expr_factor;
			}
		}

		//factor ::= TRef | SRef | Const | (RHS)
		Expr factor(Type index_type, Type data_type)
		{
			Expr expr;
			if (look.type == ID)
				expr = TSRef(index_type, data_type);
			else if (look.type == 1 || look.type == 2)
				expr =  Const(index_type, data_type);
			else if (look.type == '(')
			{
				match('(');
				expr =  RHS(index_type, data_type);
				match(')');
			}
			return expr;
			
		}  */

//TRef ::= Id < CList > [ AList ]
		  //SRef ::= Id < CList > 
		  //����TRef��SRef����ʽ���ƣ��ʽ����ߵĴ�������һ��
		Expr TSRef(Type index_type, Type data_type) {
			std::string name = "";
			std::vector<size_t> dom;
			std::vector<Expr> index;

			//ƥ���ʶ����ʹ��tok������
			Token tok = match(ID);
			match('<');
			//����CList�Ĵ����������õ����������ķ�Χ
			std::cout<<"enter Clist"<<'\n';
			dom = CList(index_type, data_type);
			std::cout<<"leave Clist"<<'\n';
			match('>');
			//�����TRef
			if (look.type == '[') {
				match('[');
				std::cout<<"Enter Alist"<<'\n';
				index = AList(index_type, data_type, dom);
				std::cout<<"leave Alist"<<'\n';
				match(']');
				//��������������
				Expr expr = Var::make(data_type, tok.getname(), index, dom);
				add_var(tok.getname(), expr);
				return expr;
			}
			//�����SRef
			else {
				index.push_back(Expr(0));
				Expr expr = Var::make(data_type, tok.getname(), index, dom);
				add_var(tok.getname(), expr);
				return expr;
			}
		}
		//Clist:::=IntVClist1 
		//Clist1::=,IntVClist1/e

		std::vector<size_t> CList(Type index_type, Type data_type) {
			std::string name = "";
			std::vector<size_t> dom;
			std::vector<Expr> index;
			//ƥ��int��ʹ��tok������
			Token tok = match(1);
			dom.push_back(tok.getvalue());
			//����CList1�Ĵ����������õ����������ķ�Χ
			std::vector<size_t> dom1 = CList1(index_type, data_type);
			dom.insert(dom.end(), dom1.begin(), dom1.end());
			return dom;
		}
		std::vector<size_t> CList1(Type index_type, Type data_type) {
			std::string name = "";
			std::vector<size_t> dom;
			std::vector<Expr> index;
			//ƥ���ʶ����ʹ��tok������
			if (look.type == ',')
			{
				match(',');
				Token tok = match(1);
				dom.push_back(tok.getvalue());
				//����CList1�Ĵ����������õ����������ķ�Χ
				std::vector<size_t> dom1 = CList1(index_type, data_type);
				dom.insert(dom.end(), dom1.begin(), dom1.end());
				return dom;
			}
			else
				return dom;
		}
		//Alist:::=IDexprAlist1 
		//Alist1::=,IdEXPRAlist1/e

		std::vector<Expr> AList(Type index_type, Type data_type, std::vector<size_t> dom) {
			std::string name = "";
			std::vector<Expr> index;
			//ƥ���ʶ����ʹ��tok������
			Expr expr = IdExpr(index_type, data_type, dom[0]);
			index.push_back(expr);
			dom.erase(dom.begin());
			//����CList1�Ĵ����������õ����������ķ�Χ
			std::vector<Expr> index1 = AList1(index_type, data_type, dom);
			for(size_t i=0;i<index1.size();i++)
				index.push_back(index1[i]);
			//index.insert(index.end(), index1.begin(), index1.end());
			return index;
		}
		std::vector<Expr> AList1(Type index_type, Type data_type, std::vector<size_t> dom) {
			std::string name = "";
			//std::vector<size_t> dom;
			std::vector<Expr> index;
			//ƥ���ʶ����ʹ��tok������
			if (look.type == ',')
			{
				match(',');
				Expr expr = IdExpr(index_type, data_type, dom[0]);
				dom.erase(dom.begin());
				index.push_back(expr);
				//����AList1�Ĵ����������õ����������ķ�Χ
				std::vector<Expr> index1 = AList1(index_type, data_type, dom);
				//index.insert(index.end(), index1.begin(), index1.end());
				for(size_t i=0;i<index1.size();i++)
					index.push_back(index1[i]);
				return index;
			}
			else
				return index;
		}
		bool is_leftindex(std::string name)
		{
			for(int i=0; i<left_name.size();i++)
			{
				if(name == left_name[i])
					return true;
			}
			return false;

		}
		// IdExpr ::= Id IdExpr1 
		//IdExpr1 ::=  + IdExpr IdExpr1| + IntV IdExpr1| * IntV IdExpr1|  // IntV IdExpr1|  % IntV IdExpr1|e
		Expr IdExpr(Type index_type, Type data_type, int domsize) {
			//ƥ���ʶ����ʹ��tok������
			Token tok = match(ID);
			if(!is_leftindex(tok.getname()))
			{
				bool tmp_flag = false;
				for(size_t i=0;i<temp_record.size();i++)
				{
					if(tok.getname()==temp_record[i])
					{
						tmp_flag = true;
						break;
					}
					
				}
				if(tmp_flag == false)
					temp_record.push_back(tok.getname());
			}
			flag = false;
			Expr dom = Dom::make(index_type, 0, domsize);
			Expr tokpr = Index::make(index_type, tok.getname(), dom, IndexType::Spatial);
			Expr expr = IdExpr1(index_type, data_type, domsize, tok.stringValue,tokpr);
			if(flag == true)
			{
				Expr expr2 = Compare::make(index_type, CompareOpType::LT, expr, IntImm::make(Type::int_scalar(32), domsize));
				total_cond.push_back(expr2);
			}
			return expr;
		}
		Expr IdExpr1(Type index_type, Type data_type, int domsize, std::string id,Expr tok) {

			
			if (look.type == '+') {
				match('+');
				if (look.type == 1) {
					Token tok2 = match(1);
					domsize -= tok2.intValue;
					Expr tok2pr = IntImm::make(Type::int_scalar(32), tok2.intValue);
					Expr exp = Binary::make(data_type, BinaryOpType::Add, tok, tok2pr);
					return IdExpr1(index_type, data_type, domsize, id, exp);
				}
				else {
					
					Expr expr = IdExpr(index_type, data_type, domsize);
					flag = true;
					Expr exp = Binary::make(data_type, BinaryOpType::Add, tok, expr);
					return IdExpr1(index_type, data_type, domsize, id,exp);
				}
				
			}
			else if (look.type == '*') {
				match('*');
				Token tok2 = match(1);
				domsize /= tok2.intValue;
				Expr tok2pr = IntImm::make(Type::int_scalar(32), tok2.intValue);
				Expr exp = Binary::make(data_type, BinaryOpType::Mul, tok, tok2pr);
				return IdExpr1(index_type, data_type, domsize, id, exp);
			}
			else if (look.type == '%') {
				match('%');
				Token tok2 = match(1);
				domsize = tok2.intValue - 1;
				Expr tok2pr = IntImm::make(Type::int_scalar(32), tok2.intValue);
				Expr exp = Binary::make(data_type, BinaryOpType::Mod, tok, tok2pr);
				return IdExpr1(index_type, data_type, domsize, id, exp);
			}
			else if (look.type == 3)// for //
			{
				match(3);
				Token tok2 = match(1);
				domsize = domsize * tok2.intValue -tok2.intValue+1;
				Expr tok2pr = IntImm::make(Type::int_scalar(32), tok2.intValue);
				Expr exp = Binary::make(data_type, BinaryOpType::Div, tok, tok2pr);
				return IdExpr1(index_type, data_type, domsize, id, exp);
			}
			else {
				Expr dom = Dom::make(index_type, 0, domsize);
				Expr tokpr = Index::make(index_type, id, dom, IndexType::Spatial);
				Myclass tmp;			
				tmp.dom = domsize;
				tmp.name = id;				
				add_index(tmp,tokpr);
				return tok;
			}
		}
		Expr Const(Type index_type, Type data_type) {
			if (look.type == 1) {
				Token tok = match(1);
				return IntImm::make(Type::int_scalar(32), tok.intValue);
			}
			else {
				Token tok = match(2);
				return FloatImm::make(Type::float_scalar(32), tok.floatValue);
			}
		}


		


	}
}

namespace Boost{
	namespace Internal{
		void init()
		{
			all_temp_names.clear();
			all_temp_size.clear();
			all_temp_vars.clear();
			temp_count = 0;


	//	std::string s ="A<16, 32>[i, j] = 2; ";  //
	//	std::vector<Token> tokens =  mylex(s);   //get tokes from token parse

		left_size.clear();  //store size of array temp
		left_index.clear();
		left_name.clear();
		temp_vars.clear();
		
		total_index.clear();
		total_size.clear();  //store all index-name-size
		total_name.clear();
		
		wrap_layer = 0;

		all_vars.clear();      //need to judge if exist!!
	 	all_vars_name.clear();
		output_vars.clear();
		input_vars.clear();

		total_cond.clear();
		flag = false;

		extra_stmt.clear();
		extra_index.clear();
        
		}

		std::string compile_project1(json json_config)
		{
			init();
			total_index.clear();
			total_size.clear();  //store all index-name-size
			total_name.clear();
			all_temp_names.clear();
			res = json_config;
        	s = res.kernel;
        	ins=res.ins;
        	outs=res.outs;
        
        	tokens = mylex(s);
			token_index = 0;  //index for tokens
			look = tokens[0]; //next token
			if (res.data_type == "int") {
				data_type = Type::int_scalar(32);
			}
			else {
				data_type= Type::float_scalar(32);
			}
		/*for(int i = 0;i<tokens.size();i++)
    	{
        std::cout<<tokens[i].type<<" ";
        if(tokens[i].type==0)
            std::cout<<"ID: "<<tokens[i].stringValue<<"\n";
        else if(tokens[i].type==1)
            std::cout<<"IntV: "<<tokens[i].intValue<<"\n";
        else if(tokens[i].type==2)
            std::cout<<"floatV: "<<tokens[i].floatValue<<"\n";
        else if(tokens[i].type==3)
            std::cout<<"operator: "<<"//"<<"\n";
        else
        {
            std::cout<<"operator: "<<(char)tokens[i].type<<"\n";
        }
        
   		 }

		*/
        std::vector<Stmt> all_stmts = P(index_type, data_type);

        //std::cout<<"P is success!"<<"\n";

        
       
		for (size_t i = 0; i < ins.size(); i++) {
			for (size_t j = 0; j < all_vars.size(); j++) {
				if (ins[i] == all_vars_name[j]) {
					input_vars.push_back(all_vars[j]);
				}
			}
		}
		for (size_t i = 0; i < outs.size(); i++) {
			bool flag1 = false;
			for (size_t k = 0; k < ins.size(); k++) {
				if (outs[i] == ins[k]) {
					flag1 = true;
					break;
					}
			}
			if (flag1 == true) {
				continue;
			}
			for (size_t j = 0; j < all_vars.size(); j++) {
				if (outs[i] == all_vars_name[j]) {
					output_vars.push_back(all_vars[j]);
				}
			}
		}
        // kernel
        Group kernel = Kernel::make(res.name, input_vars, output_vars, all_stmts, KernelType::CPU);

        // visitor
        IRVisitor visitor;
        kernel.visit_group(&visitor);

        // mutator
        IRMutator mutator;
        kernel = mutator.mutate(kernel);

         // printer
        IRPrinter printer;
        std::string code = printer.print(kernel);

        //print_temp();   
        //for(int i=0;i<all_temp_names.size();i++)
        //	std::cout<<all_temp_names[i];

		return code;
		}
	}
	}