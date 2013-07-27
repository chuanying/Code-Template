//�㷨α���룺
MorrisInOrder():
	while û�н���
		�����ǰ�ڵ�û������
			���ʸýڵ�
			ת���ҽڵ�
		����
			�ҵ����������ҽڵ㣬��ʹ���ҽڵ����ָ��ָ��ǰ�ڵ�
			ת�������ڵ�


//version 1: cpp
void bst_morris_inorder(struct bst_node *root){
	struct bst_node *p = root, *tmp;
	while(p){
		if(p->left == NULL){
			printf("%d", p->key);
			p = p->right;
		}
		else{
			tmp = p->left;
			while(tmp->right != NULL && tmp->right != p)
				tmp = tmp->right;
			if(tmp->right == NULL){
				tmp->right = p;
				p = p->left;
			}
			else{ //tmp->right == p
				printf("%d", p->key);
				tmp->right = NULL;
				p = p->right;
			}
		}
	}
}

//version 2: template
template<class T>
void BST<T>::MorrisInorder() {
	BSTNode<T> *p = root, *tmp;
	while(p != 0)
		if(p->left == 0){
			visit(p);
			p = p->right;
		}
		else{
			tmp = p->left;
			while(tmp->right != 0 && tmp->right != p)
				tmp = tmpp->right;
			if(tmp->right == 0){
				tmp->right = p;
				p = p->left;
			}
			else{
				visit(p);
				tmp->right = 0;
				p = p->right;
			}
		}
}