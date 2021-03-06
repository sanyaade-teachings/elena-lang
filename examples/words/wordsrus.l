//#������ basic'* = std'basic'*.
//#������ list'*  = std'collections'*.
//#������ ctrl'*  = std'patterns'*.
//
//#������ text'*  = ext'text'*.
//#������ io'*   = ext'io'*.
//
//// --- WordList ---
//
//#����� WordList
//{
//    #���� ������.
//    #���� �����.
//
//    #����� new
//    [
//        ������ := list'List.
//        ����� := basic'Integer << 0.
//    ]
//
//    #����� += �����
//    [
//        ����� += 1.
//        
//        #����(ctrl'Searching::������ == �����)!
//            [ ������ += basic'LiteralType of:�����. ].
//    ]
//
//    #����� $��������� = �����.
//
//    #����� $�������������� = ������ array'Count.
//
//    #���������� (������).
//}
//
//// --- Program ---
//
//#������ ��������� =>
//[
//    'program'output << "������� �����(����� ��������� ������ ������� ENTER):%n".
//
//    #��� ����� := basic'String.
//    #��� ���� := basic'Boolean << basic'True.
//
//    #��� aReader := #����������(�����)
//    {
//        += ��������
//        [
//            #var ������ := basic'LiteralType of:��������.
//
//            #if (������ array'Count != 0)?
//                [ ����� += ��������. ]
//                | [ ���� << basic'False. ].
//        ]
//    }.
//
//    #���� ����?
//    [    
//        'program'Input >> aReader.
//
//        ����� += " ".
//    ].
//
//    #��� ������ := WordList.
//    
//    #������(ctrl'EEnumeration, text'EWords, �����) 
//        run: aWord =>(������ += aWord).
//
//    'program'output << "����� " << ������ $�������������� << " ���������� ���� �� " << ������ $���������.
//    'program'output << "%n������ ���������� ����:%n".
//
//    'program'output << #������(io'EListPrinting, ������).
//
//    'program'input char'get. // wait for any key
//].
