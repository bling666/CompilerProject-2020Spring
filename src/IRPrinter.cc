/*
 * MIT License
 * 
 * Copyright (c) 2020 Size Zheng

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "IRPrinter.h"
#include "kernel.h"
/*
extern std::vector<std::string> all_temp_names;
extern std::vector<std::vector<size_t>> all_temp_size;
void print_temp()
        {
            for(int i=0; i<all_temp_names.size(); i++)
            {
                std::cout <<  "float " << all_temp_names[i];
                int dim = all_temp_size[i].size();
                for(int j = 0;j < dim; j++)
                    std::cout << "[" << all_temp_size[i][j] << "]";
                std::cout << ";\n";
            }
            
            
        }

*/
namespace Boost
{

    namespace Internal
    {

        std::string IRPrinter::print(const Expr &expr)
        {
            oss.clear();
            expr.visit_expr(this);
            return oss.str();
        }

        std::string IRPrinter::print(const Stmt &stmt)
        {
            oss.clear();
            stmt.visit_stmt(this);
            return oss.str();
        }

        std::string IRPrinter::print(const Group &group)
        {
            oss.clear();
            group.visit_group(this);
            return oss.str();
        }

        void IRPrinter::visit(Ref<const IntImm> op)
        {
            //oss << "(" << op->type() << " " << op->value() << ")";
            oss << op->value();
        }

        void IRPrinter::visit(Ref<const UIntImm> op)
        {
            //oss << "(" << op->type() << " " << op->value() << ")";
            oss << op->value();
        }

        void IRPrinter::visit(Ref<const FloatImm> op)
        {
            //oss << "(" << op->type() << " " << op->value() << ")";
            oss << op->value();
        }

        void IRPrinter::visit(Ref<const StringImm> op)
        {
            //oss << "(" << op->type() << " " << op->value() << ")";
            oss << op->value();
        }

        void IRPrinter::visit(Ref<const Unary> op)
        {
            if (op->op_type == UnaryOpType::Neg)
            {
                oss << "-";
            }
            else if (op->op_type == UnaryOpType::Not)
            {
                oss << "!";
            }
            (op->a).visit_expr(this);
        }

        void IRPrinter::visit(Ref<const Binary> op)
        {
            oss << "(";
            (op->a).visit_expr(this);
            if (op->op_type == BinaryOpType::Add)
            {
                oss << " + ";
            }
            else if (op->op_type == BinaryOpType::Sub)
            {
                oss << " - ";
            }
            else if (op->op_type == BinaryOpType::Mul)
            {
                oss << " * ";
            }
            else if (op->op_type == BinaryOpType::Div)
            {
                oss << " / ";
            }
            else if (op->op_type == BinaryOpType::Mod)
            {
                oss << " % ";
            }
            else if (op->op_type == BinaryOpType::And)
            {
                oss << " && ";
            }
            else if (op->op_type == BinaryOpType::Or)
            {
                oss << " || ";
            }
            (op->b).visit_expr(this);
            oss << ")";
        }

        void IRPrinter::visit(Ref<const Compare> op)
        {
            (op->a).visit_expr(this);
            if (op->op_type == CompareOpType::LT)
            {
                oss << " < ";
            }
            else if (op->op_type == CompareOpType::LE)
            {
                oss << " <= ";
            }
            else if (op->op_type == CompareOpType::EQ)
            {
                oss << " == ";
            }
            else if (op->op_type == CompareOpType::GE)
            {
                oss << " >= ";
            }
            else if (op->op_type == CompareOpType::GT)
            {
                oss << " > ";
            }
            else if (op->op_type == CompareOpType::NE)
            {
                oss << " != ";
            }
            (op->b).visit_expr(this);
        }

        void IRPrinter::visit(Ref<const Select> op)
        {
            oss << "select(";
            (op->cond).visit_expr(this);
            oss << ", ";
            (op->true_value).visit_expr(this);
            oss << ", ";
            (op->false_value).visit_expr(this);
            oss << ")";
        }

        void IRPrinter::visit(Ref<const Call> op)
        {
            oss << "call_";
            if (op->call_type == CallType::Pure)
            {
                oss << "pure";
            }
            else if (op->call_type == CallType::SideEffect)
            {
                oss << "side_effect";
            };
            oss << "(" << op->func_name;
            for (size_t i = 0; i < op->args.size(); ++i)
            {
                oss << ", ";
                op->args[i].visit_expr(this);
            }
            oss << ")";
        }

        void IRPrinter::visit(Ref<const Cast> op)
        {
            oss << "cast<" << op->new_type << ">(";
            (op->val).visit_expr(this);
            oss << ")";
        }

        void IRPrinter::visit(Ref<const Ramp> op)
        {
            oss << "ramp(";
            (op->base).visit_expr(this);
            oss << ", " << op->stride << ", " << op->lanes << ")";
        }

        void IRPrinter::visit(Ref<const Var> op)
        {
            if (print_arg)
            {
                oss << op->type_;
                oss << " (&" << op->name << ")";
                for (size_t i = 0; i < op->shape.size(); ++i)
                {
                    if (op->shape[i] == 1)
                        continue;
                    oss << "[";
                    oss << op->shape[i];
                    oss << "]";
                }
            }
            else
            {
                oss << op->name;
                //if(op->name =="alpha" || op->name == "beta")
                //return;
                if (op->args.size() == 1 && op->args[0]->node_type() == IRNodeType::IntImm)
                    return;
                for (size_t i = 0; i < op->args.size(); ++i)
                {
                    oss << "[";
                    op->args[i].visit_expr(this);
                    oss << "]";
                }
            }
        }

        void IRPrinter::visit(Ref<const Dom> op)
        {
            if (domflag == 0)
                (op->begin).visit_expr(this);
            else
                (op->extent).visit_expr(this);
        }

        void IRPrinter::visit(Ref<const Index> op)
        {
            if (print_range)
            {
                oss << "(int " << op->name << " = ";
                domflag = 0;
                (op->dom).visit_expr(this);
                oss << ";" << op->name << " < ";
                domflag = 1;
                (op->dom).visit_expr(this);
                oss << ";" << op->name << "++) ";
            }
            else
            {
                oss << op->name;
            }

            /*if (print_range) {
        oss << "<";
        if (op->index_type == IndexType::Spatial) {
            oss << "spatial";
        } else if (op->index_type == IndexType::Reduce) {
            oss << "reduce";
        } else if (op->index_type == IndexType::Unrolled) {
            oss << "unrolled";
        } else if (op->index_type == IndexType::Vectorized) {
            oss << "vectorized";
        } else if (op->index_type == IndexType::Block) {
            oss << "block";
        } else if (op->index_type == IndexType::Thread) {
            oss << "thread";
        }
        oss << "> in ";
        (op->dom).visit_expr(this);
    }*/
        }

        void IRPrinter::visit(Ref<const LoopNest> op)
        {
            print_range = true;
            for (auto index : op->index_list)
            {
                print_indent();
                oss << "for ";
                index.visit_expr(this);
                oss << "{\n";
                enter();
            }
            print_range = false;
            for (auto body : op->body_list)
            {
                body.visit_stmt(this);
            }
            for (auto index : op->index_list)
            {
                exit();
                print_indent();
                oss << "}\n";
            }
        }

        void IRPrinter::visit(Ref<const IfThenElse> op)
        {
            print_indent();
            oss << "if (";
            (op->cond).visit_expr(this);
            oss << ") {\n";
            enter();
            (op->true_case).visit_stmt(this);
            exit();
            print_indent();
            oss << "}\n";
            /*enter();
    (op->false_case).visit_stmt(this);
    exit();
    print_indent();
    oss << "}\n";*/
        }

        void IRPrinter::visit(Ref<const Move> op)
        {
            print_indent();
            (op->dst).visit_expr(this);
            oss << " = ";
            /*if (op->move_type == MoveType::HostToDevice) {
        oss << "host_to_device";
    } else if (op->move_type == MoveType::MemToShared) {
        oss << "mem_to_shared";
    } else if (op->move_type == MoveType::SharedToMem) {
        oss << "shared_to_mem";
    } else if (op->move_type == MoveType::MemToLocal) {
        oss << "mem_to_local";
    } else if (op->move_type == MoveType::LocalToMem) {
        oss << "local_to_mem";
    } else if (op->move_type == MoveType::SharedToLocal) {
        oss << "shared_to_local";
    } else if (op->move_type == MoveType::LocalToShared) {
        oss << "local_to_shared";
    } else if (op->move_type == MoveType::SharedToShared) {
        oss << "shared_to_shared";
    } else if (op->move_type == MoveType::MemToMem) {
        oss << "mem_to_mem";
    } else if (op->move_type == MoveType::LocalToLocal) {
        oss << "local_to_local";
    }
    oss << "> ";*/
            (op->src).visit_expr(this);
            oss << ";\n";
        }

        void IRPrinter::visit(Ref<const Kernel> op)
        {
            print_indent();
            /*if (op->kernel_type == KernelType::CPU) {
        oss << "<CPU>";
    } else if (op->kernel_type == KernelType::GPU) {
        oss << "<GPU>";
    }*/
            oss << "void " << op->name << "(";
            print_arg = true;
            for (size_t i = 0; i < op->inputs.size(); ++i)
            {
                op->inputs[i].visit_expr(this);
                if (i < op->inputs.size() - 1)
                {
                    oss << ", ";
                }
            }
            for (size_t i = 0; i < op->outputs.size(); ++i)
            {
                if (op->inputs.size() != 0 && i == 0)
                    oss << ", ";
                op->outputs[i].visit_expr(this);
                if (i < op->outputs.size() - 1)
                {
                    oss << ", ";
                }
            }
            print_arg = false;
            oss << ") {\n";
            enter();

            std::string temp_str = print_temp();
            oss << temp_str;
            for (auto stmt : op->stmt_list)
            {
                stmt.visit_stmt(this);
            }
            exit();
            oss << "}\n";
        }

    } // namespace Internal

} // namespace Boost
