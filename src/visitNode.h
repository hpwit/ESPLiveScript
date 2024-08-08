void _visittypeNode(NodeToken *nd) {}
void _visitnumberNode(NodeToken *nd) {}
void _visitbinOpNode(NodeToken *nd) {}
void _visitunitaryOpNode(NodeToken *nd) {}
void _visitoperatorNode(NodeToken *nd) {}
void _visitProgram(NodeToken *nd) {}
void _visitglobalVariableNode(NodeToken *nd) {}
void _visitlocalVariableNode(NodeToken *nd) {}
void _visitblockStatementNode(NodeToken *nd) {
    for (int i = 0; i < nd->children.size(); i++)
    {

            nd->getChildAtPos(i)->visitNode();
            // f = f + g.f;

    }
    nd->clear();
}
void _visitdefFunctionNode(NodeToken *nd) {
    printf("visit function %s\n",nd->getTokenText());
}
void _visitstatementNode(NodeToken *nd) {}
void _visitprogramNode(NodeToken *nd) {
    printf("visit program\n");
    point_regnum = 4;
    content.begin();
    header.begin();
    header.addAfter("stack:");
    header.addAfter(".bytes 60");
    header.addAfter("stackr:");
    header.addAfter(".bytes 60");

    // header.addAfter("__basetime:");
    // header.addAfter(".bytes 4");
    register_numr.clear();
    register_numl.clear();
    register_numl.push(15);
    register_numr.push(15);
    for (int i = 0; i < nd->children.size(); i++)
    {

#ifndef __MEM_PARSER
            if (nd->getChildAtPos(i)->_nodetype != defFunctionNode && nd->getChildAtPos(i)->_nodetype != defAsmFunctionNode)
            {
#endif
                nd->getChildAtPos(i)->visitNode();
#ifndef __MEM_PARSER
            } // NEW
#endif
    }
}
void _visitassignementNode(NodeToken *nd) {}
void _visitcomparatorNode(NodeToken *nd) {}
void _visitcallFunctionNode(NodeToken *nd) {}
void _visitforNode(NodeToken *nd) {}
void _visitargumentNode(NodeToken *nd) {}
void _visitextGlobalVariableNode(NodeToken *nd) {}
void _visitextDefFunctionNode(NodeToken *nd) {
    printf("visit1 function %s\n",nd->getTokenText());
}
void _visitextCallFunctionNode(NodeToken *nd) {}
void _visitreturnArgumentNode(NodeToken *nd) {}
void _visitvariableDeclarationNode(NodeToken *nd) {}
void _visitdefExtFunctionNode(NodeToken *nd) {
     printf("visit externazl function %s\n",nd->getTokenText());
}
void _visitinputArgumentsNode(NodeToken *nd) {}
void _visitdefExtGlobalVariableNode(NodeToken *nd) {}
void _visitdefGlobalVariableNode(NodeToken *nd) {}
void _visitdefLocalVariableNode(NodeToken *nd) {}
void _visitstoreLocalVariableNode(NodeToken *nd) {}
void _visitstoreGlobalVariableNode(NodeToken *nd) {}
void _visitstoreExtGlocalVariableNode(NodeToken *nd) {}
void _visitifNode(NodeToken *nd) {}
void _visitelseNode(NodeToken *nd) {}
void _visitwhileNode(NodeToken *nd) {}
void _visitreturnNode(NodeToken *nd) {}
void _visitdefAsmFunctionNode(NodeToken *nd) {
    header.addAfter(string_format(".global %s", nd->getTokenText()));
    content.addAfter(string_format("%s:", nd->getTokenText()));
    // content.addAfter(string_format("entry a1,%d", 80)); // ((nd->stack_pos) / 8 + 1) * 8)
    if (nd->children.size() >= 3)
 {
            nd->getChildAtPos(2)->visitNode();
  }          // f = f + g.f;
            // h = h + g.header;
        
}
void _visitstringNode(NodeToken *nd) {
    string target=string(nd->getTokenText());
    content.addAfter(string_format("%s", target.substr(1, target.size() - 2).c_str()));
}
void _visitchangeTypeNode(NodeToken *nd) {}
void _visitimportNode(NodeToken *nd) {}
void _visitcontinueNode(NodeToken *nd) {}
void _visitbreakNode(NodeToken *nd) {}
void _visitUnknownNode(NodeToken *nd) {}