#include "IR.h"
#include "IRMutator.h"
#include "IRVisitor.h"
#include "IRPrinter.h"
#include "type.h"
#include "token.h"
#include "dx.h"
#include "lex.h"
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <iostream>
#include <cstring>

namespace Boost
{
    namespace Internal
    {
        namespace dx
        {
            int ID = 0; //define ID 0

            std::string s;
            std::vector<std::string> ins;
            std::vector<std::string> outs;
            std::vector<std::string> grad_tos;
            std::vector<Token> tokens;

            std::vector<Token> need_to_handle;
            std::vector<Token> handle_result;
            std::vector<Token> left_row;
            std::vector<Token> merge_result;
            std::vector<Token> leftdiff;
            Token nowdiff;
            int token_index; //index for tokens
            Token look;      //next token

			bool flag1 = 0;

            std::vector<Token> newtok;
            void autodiff(std::vector<Token> tok)
            {
                int size = tok.size();
                int sum = 0;
			
                newtok.insert(newtok.end(), leftdiff.begin(), leftdiff.end());
                for (int i = 0; i < size; i++)
                {
				
                    Token tmp = tok[i];
                    if (tmp.type == 0)
                    {
                        if (tmp.stringValue == nowdiff.stringValue)
                        {
						
                            int start = i;
                            if (sum == 0)
                            {
                                newtok.pop_back();
                                Token tmp1 = tok[i];
                                while (tmp1.type != ']')
                                {
                                    i++;
                                    tmp1 = tok[i];
                                }
                                i++;
                            }
                            sum++;
							std::cout << "4\n" ;
                            if (left_row.empty())
                            {
								for (int j = 0; j < tok.size(); j++)
									std::cout << tok[j].type;
								std::cout << "\n"<<start;
								std::cout << " "<<i<<"\n";
                                left_row.insert(left_row.end(), tok.begin() + start, tok.begin() + i);
                            }
                        }
                    }
					if (i == size)
						break;
					
                    newtok.push_back(tok[i]);
                }
                Token mytok2;
				
                if (sum == 0)
                {
                    newtok.clear();
                    mytok2.type = 2;
                    mytok2.floatValue = 0.0;
                    newtok.push_back(mytok2);
                }
                else if (sum == 1)
                {
                }
                else
                {
                    Token mytok;
                    mytok.type = '*';
                    newtok.push_back(mytok);
                    
                    mytok2.type = 1;
                    mytok2.intValue = sum;
                    newtok.push_back(mytok2);
                }
            }
			class change {
			public:
				Token need_tochange;
				std::vector<Token> Change_to;
			};
			std::vector<change> Change;
			std::string change_token() {
				bool flag = 1;
				std::vector<Token> change_result;
				int count = 0;
				Change.clear();
				for (int i = 0; i < merge_result.size(); i++)
				{
					if (flag == 1) {
						if (merge_result[i].type == '+') {
							count++;
							change tmp;
							Token tmp2;
							Token tmp3;
							tmp3.type = '-';
							tmp2.type = 0;
							tmp.need_tochange = merge_result[i - 1];
							change_result.pop_back();
							std::string temp_name = "tmp";
							temp_name += std::to_string(count);
							tmp2.stringValue = temp_name;
							change_result.push_back(tmp2);
							tmp.Change_to.push_back(tmp2);
							tmp.Change_to.push_back(tmp3);
							tmp.Change_to.push_back(merge_result[i + 1]);
							Change.push_back(tmp);
							i += 2;
						}
						else if (merge_result[i].type == '-') {
							count++;
							change tmp;
							Token tmp2;
							Token tmp3;
							tmp3.type = '+';
							tmp2.type = 0;
							tmp.need_tochange = merge_result[i - 1];
							change_result.pop_back();
							std::string temp_name = "tmp";
							temp_name += std::to_string(count);
							tmp2.stringValue = temp_name;
							change_result.push_back(tmp2);
							tmp.Change_to.push_back(tmp2);
							tmp.Change_to.push_back(tmp3);
							tmp.Change_to.push_back(merge_result[i + 1]);
							Change.push_back(tmp);
							i += 2;
						}
						else if (merge_result[i].type == '*') {
							count++;
							change tmp;
							Token tmp2;
							Token tmp3;
							tmp3.type = 3;
							tmp2.type = 0;
							tmp.need_tochange = merge_result[i - 1];
							change_result.pop_back();
							std::string temp_name = "tmp";
							temp_name += std::to_string(count);
							tmp2.stringValue = temp_name;
							change_result.push_back(tmp2);
							tmp.Change_to.push_back(tmp2);
							tmp.Change_to.push_back(tmp3);
							tmp.Change_to.push_back(merge_result[i + 1]);
							Change.push_back(tmp);
							i += 2;
						}
						else if (merge_result[i].type == 3) {
							count++;
							change tmp;
							Token tmp2;
							Token tmp3;
							tmp3.type = '*';
							tmp2.type = 0;
							tmp.need_tochange = merge_result[i - 1];
							change_result.pop_back();
							std::string temp_name = "tmp";
							temp_name += std::to_string(count);
							tmp2.stringValue = temp_name;
							change_result.push_back(tmp2);
							tmp.Change_to.push_back(tmp2);
							tmp.Change_to.push_back(tmp3);
							tmp.Change_to.push_back(merge_result[i + 1]);
							Change.push_back(tmp);
							i += 2;
						}
						else if (merge_result[i].type == '%') {
							count++;
							change tmp;
							Token tmp2;
							tmp2.type = 0;
							tmp.need_tochange = merge_result[i - 1];
							change_result.pop_back();
							std::string temp_name = "tmp";
							temp_name += std::to_string(count);
							tmp2.stringValue = temp_name;
							change_result.push_back(tmp2);
							tmp.Change_to.push_back(tmp2);
							Change.push_back(tmp);
							i += 2;
						}
					}
					bool flag2 = 0;

					if (merge_result[i].type == '=')
						flag = 0;
					if (flag == 0) {
						if (merge_result[i].type == 0) {

							for (int j = 0; j < Change.size(); j++) {
								if (Change[j].need_tochange.stringValue == merge_result[i].stringValue) {
									if (flag2 == 1) {
										Token tmp3;
										tmp3.type = '+';
										change_result.push_back(tmp3);
									}
									change_result.insert(change_result.end(), Change[j].Change_to.begin(), Change[j].Change_to.end());
									flag2 = 1;
								}
							}
						}
					}
					if (flag2 == 0) {
					change_result.push_back(merge_result[i]);
					}

				}
				std::string final_result;
				for (int i = 0; i < change_result.size(); i++)
				{
					int type = change_result[i].type;
					if (type == 0)
						final_result.append(change_result[i].stringValue.c_str());
					else if (type == 1)
						final_result.append(std::to_string(change_result[i].intValue));
					else if (type == 2)
						if (change_result[i].floatValue == 0.0)
							final_result.append(std::to_string(0.0));
						else
							final_result.append(std::to_string(change_result[i].floatValue));
					else if (type == 3)
						final_result.append("//");
					else
					{
						final_result = final_result + ((char)change_result[i].type);
					}
				}
				std::cout << final_result << std::endl;
				return final_result;
			}
            Token match(int k)
            {
                if (tokens[token_index].type != k)
                {
                    std::cout << "match is wrong!";
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
                if (token_index == tokens.size() - 1)
                    return look;
                if (k == 0 || k == 1 || k == 2)
                {
                    Token tmp_token = tokens[token_index];
                    need_to_handle.push_back(tokens[token_index]);
                    token_index++;
                    look = tokens[token_index];
                    return tmp_token;
                }
                else
                {
                    if (flag1==0)
                        need_to_handle.push_back(tokens[token_index]);
                    token_index++;
                    look = tokens[token_index];
                    return look;
                }
            }

            void PrintToken(Token s)
            {
                int type = s.type;
                if (type == 0)
                    printf("%s", s.stringValue.c_str());
                else if (type == 1)
                    printf("%d", s.intValue);
                else if (type == 2)
                    printf("%f", s.floatValue);
                else
                {
                    printf("%c", s.type);
                }
            }
            //P ::= S*
            void P()
            {

                while (look.type == ID)
                {
                    std::cout << "enter S" << '\n';
                    S();
                    match(';');
                    std::cout << "leave S" << '\n';
                    
                };
                std::cout << "P is success" << '\n';
            }

            //S ::= LHS = RHS
            void S()
            {
                std::cout << "enter LHS" << '\n';
                LHS();
                need_to_handle[0].stringValue = "d" + need_to_handle[0].stringValue;
                leftdiff.insert(leftdiff.end(), need_to_handle.begin(), need_to_handle.end());
                std::cout << "leave LHS" << '\n';

                match('=');
                need_to_handle.clear();
                Token mytok;
                mytok.type = '*';
                need_to_handle.push_back(mytok);
                RHS();
                int flag = 0;
                for(int i= 0;i<handle_result.size();i++)
                {
                    int type = handle_result[i].type;
                    if(type == 0&&handle_result[i].stringValue == nowdiff.stringValue)
                    {
                        flag = true;
                    }
                }
                if(!flag)
                {
                    int pos = -1;
                    for(int i = 0;i<ins.size();i++)
                    {
                        if(ins[i]==nowdiff.stringValue)
                        {
                            pos = i;
                        }
                    }
                    if(pos !=-1)
                        ins.erase(ins.begin()+pos);
                }
                outs[0] = "d"+outs[0];
                outs.push_back("d"+nowdiff.stringValue);
                left_row[0].stringValue = "d" + left_row[0].stringValue;
                merge_result.insert(merge_result.end(), left_row.begin(), left_row.end());
                Token equal_token;
                equal_token.type = '=';
                merge_result.push_back(equal_token);
                merge_result.insert(merge_result.end(), handle_result.begin(), handle_result.end());
                Token semilon_token;
                semilon_token.type = ';';
                merge_result.push_back(semilon_token);
                for (int i = 0; i < merge_result.size(); i++)
                    PrintToken(merge_result[i]);
                printf("\n");
            }
            //LHS ::= TRef
            void LHS()
            {
                std::cout << "enter TSRef" << '\n';
                TSRef();
                std::cout << "leave TSRef" << '\n';
            }

            //RHS1
            void RHS1()
            {

                if (look.type == '+')
                {
					flag1 = 1;
                    match('+');
					flag1 = 0;
                    RHS();
                }
                else if (look.type == '*')
                {
                    match('*');
                    term();
                }
                else if (look.type == '-')
                {
					flag1 = 1;
                    match('-');
					flag1 = 0;
                    RHS();
                }
                else if (look.type == '%')
                {
                    match('%');
                    term();
                }
                else if (look.type == '/')
                {
                    match('/');
                    term();
                }
                else if (look.type == 3)
                {
                    match(3);
                    term();
                }
                else
                {
                    std::cout << "something is wrong!\n";
                }
            }

            //RHS::= RHS + term | RHS - term | term
            void RHS()
            {
                term();
                for (int i = 0; i < need_to_handle.size(); i++)
                {
                    PrintToken(need_to_handle[i]);
                }
                printf("\n");
                autodiff(need_to_handle);
                for (int i = 0; i < newtok.size(); i++)
                {
                    PrintToken(newtok[i]);
                }
                printf("\n");
                handle_result.insert(handle_result.end(), newtok.begin(), newtok.end());
                need_to_handle.clear();
                newtok.clear();
                Token mytok;
                mytok.type = '*';
                need_to_handle.push_back(mytok);

                if (look.type == '+' || look.type == '-')
                {
                    handle_result.push_back(look);
                    RHS1();
                }
            }

            //term::= term*factor | term/factor | term%factor | term//factor | factor
            //factor::= Const | (RHS) | TRef | SRef
            void term()
            {

                if (look.type == 1 || look.type == 2)
                {
                    Const();
                    if (look.type == '*' || look.type == '/' || look.type == '%' || look.type == '3')
                        RHS1();
                }
                else if (look.type == '(')
                {
                    match('(');
                    RHS();
                    match(')');
                    if (look.type == '*' || look.type == '/' || look.type == '%' || look.type == '3')
                        RHS1();
                }
                else
                {
                    TSRef();
                    if (look.type == '*' || look.type == '/' || look.type == '%' || look.type == '3')
                        RHS1();
                }
            }

            //TRef ::= Id < CList > [ AList ]
            //SRef ::= Id < CList >
            //����TRef��SRef����ʽ���ƣ��ʽ����ߵĴ�������һ��
            void TSRef()
            {

                Token tok = match(ID);
                match('<');

                std::cout << "enter Clist" << '\n';
                CList();
                std::cout << "leave Clist" << '\n';
                match('>');

                if (look.type == '[')
                {
                    match('[');
                    std::cout << "Enter Alist" << '\n';
                    AList();
                    std::cout << "leave Alist" << '\n';
                    match(']');
                }
            }
            //Clist:::=IntVClist1
            //Clist1::=,IntVClist1/e

            void CList()
            {
                Token tok = match(1);
                CList1();
            }
            void CList1()
            {
                if (look.type == ',')
                {
                    match(',');
                    Token tok = match(1);
                    CList1();
                }
            }
            //Alist:::=IDexprAlist1
            //Alist1::=,IdEXPRAlist1/e

            void AList()
            {
                IdExpr();
                AList1();
            }
            void AList1()
            {
                //ƥ���ʶ����ʹ��tok������
                if (look.type == ',')
                {
                    match(',');
                    IdExpr();
                    AList1();
                }
                else
                {
                }
            }
            // IdExpr ::= Id IdExpr1
            //IdExpr1 ::=  + IdExpr IdExpr1| + IntV IdExpr1| * IntV IdExpr1|  // IntV IdExpr1|  % IntV IdExpr1|e
            void IdExpr()
            {
                Token tok = match(ID);
                IdExpr1();
            }
            void IdExpr1()
            {

                if (look.type == '+')
                {
                    match('+');
                    if (look.type == 1)
                    {
                        Token tok2 = match(1);
                        return IdExpr1();
                    }
                    else
                    {

                        IdExpr();
                        IdExpr1();
                    }
                }
                else if (look.type == '*')
                {
                    match('*');
                    Token tok2 = match(1);
                    IdExpr1();
                }
                else if (look.type == '%')
                {
                    match('%');
                    Token tok2 = match(1);
                    IdExpr1();
                }
                else if (look.type == 3) // for //
                {
                    match(3);
                    Token tok2 = match(1);
                    IdExpr1();
                }
                else
                {
                }
            }
            void Const()
            {
                if (look.type == 1)
                {
                    Token tok = match(1);
                }
                else
                {
                    Token tok = match(2);
                }
            }
        } // namespace dx
    }     // namespace Internal

} // namespace Boost

namespace Boost
{
    namespace Internal
    {
        namespace dx
        {
            std::string te(std::string kernel,std::vector<std::string> & in,std::vector<std::string> & out, std::vector<std::string> & grad_to)
            {
            s.clear();
            ins.clear();
            outs.clear();

            tokens.clear();

            need_to_handle.clear();
            handle_result.clear();
            left_row.clear();
            merge_result.clear();
            leftdiff.clear();

            ins = in;
            outs = out;
            grad_tos = grad_to;
            tokens = mylex(kernel);
            token_index = 0;  //index for tokens
            look = tokens[0]; //next token
            nowdiff.type = 0;
            nowdiff.stringValue = grad_to[0];
            P();
            std::string final_result;
         /*   for(int i = 0;i<merge_result.size();i++)
            {
                    int type = merge_result[i].type;
                    if (type == 0)
                        final_result.append(merge_result[i].stringValue.c_str());
                    else if (type == 1)
                        final_result.append(std::to_string(merge_result[i].intValue));
                    else if (type == 2)
                        if(merge_result[i].floatValue == 0.0)
                            final_result.append(std::to_string(0.0));
                        else
                        final_result.append(std::to_string(merge_result[i].floatValue));
                    else
                    {
                        final_result = final_result+((char)merge_result[i].type);
                    }
            }*/
			final_result=change_token();
            in.clear();
            in.insert(in.end(),ins.begin(),ins.end());
            out.clear();
            out.insert(out.end(),outs.begin(),outs.end());
            std::cout<<final_result<<std::endl;
                return final_result;
            }
            
        } // namespace dx
    }     // namespace Internal
} // namespace Boost