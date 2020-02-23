# cetus-sqlparser

##  简介
cetus-sqlparser是从Cetus项目中剥离出的一个简单的SQL解析器。关于Cetus更多的介绍，可以跳转到cetus主页：
https://github.com/cetus-tools/cetus

## 使用简介

### 安装
我认为所有使用此项目的人员都是具备基本的c/c++代码能力的，所以此项目不提供编译好的安装包，所有的安装使用都需要你来
动手实现。
```asm
#安装依赖
yum install cmake gcc glib2-devel flex
#下载代码
git clone   https://github.com/sunashe/cetus-sqlparser.git

cd cetus-sqlparser
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local/cetus-sqlparser -DSIMPLE_PARSER=OFF

#你可以手动建立软连接，或者在自己的项目中去指定库地址。
ln -s /usr/local/cetus-sqlparser/lib/libsqlparser.so /usr/lib64/
```

### 使用示例
我们以一个简单的示例来展示cetus-sqlparser的用法，假设你想使用cetus-sqlparser来解析一条SQL语句，并且输出这条SQL
语句中的表名，如下：
```cpp
//
// Created by ashe on 2020/2/11.
//
#include <stdio.h>
#include "sql-context.h"
#include <glib.h>
#include "sql-property.h"
#include "sql-expression.h"
void print_sql_table_names(GString* sql)
{

	sql_context_t* context = g_new0(sql_context_t, 1);
	sql_context_parse_len(context, sql);
	int i=0;
	switch(context->stmt_type)
	{
		case STMT_INSERT:
		{
			sql_insert_t *sql_insert = (sql_insert_t*)context->sql_statement;
			for(i=0;i<sql_insert->table->len;i++)
			{
				sql_src_item_t* src =g_ptr_array_index(sql_insert->table,i);
				printf("table name %s\n",src->table_name);
			}
			break;
		}
		case STMT_DELETE:
		{
			sql_delete_t *sql_delete = (sql_delete_t*)context->sql_statement;
			for(i=-0;i < sql_delete->from_src->len;i++)
			{
				sql_src_item_t* src =  g_ptr_array_index(sql_delete->from_src,i);
				printf("table name %s\n",src->table_name);
			}
			break;
		}
		case STMT_UPDATE:
		{
			sql_update_t *sql_update = (sql_update_t*)context->sql_statement;
			for (i = 0; i < sql_update->table_reference->table_list->len; ++i)
			{
				sql_src_item_t *src = g_ptr_array_index(sql_update->table_reference->table_list, i);
				printf("table name %s\n",src->table_name);
			}
			break;
		}
		case STMT_SELECT:
		{
			sql_select_t *sql_select = (sql_select_t*)context->sql_statement;
			for (i = 0; i < sql_select->from_src->len; ++i)
			{
				sql_src_item_t *src = g_ptr_array_index(sql_select->from_src, i);
				printf("table name %s\n",src->table_name);
			}
			break;
		}
		default:
		{
			printf("ignore sql\n");
		}
	}
	sql_context_destroy(context);
	g_free(context);
}

int main()
{
	int i=0;
	GString* sql_select = g_string_new("select * from t1 as C join t2 as D on C.id = D.id");
	g_string_append_c(sql_select, '\0'); /* 2 more NULL for lexer EOB */
	g_string_append_c(sql_select, '\0');
	print_sql_table_names(sql_select);
	g_string_free(sql_select,TRUE);
	return 0;
}

```
编译运行
```asm
-rw-rw-r--. 1 parallels parallels 1896 2月  23 15:04 ashe.c
[parallels@Clion-CentOS-7 test]$ gcc ashe.c -I/usr/local/cetus-sqlparser/include  -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include  -L/usr/local/cetus-sqlparser/lib -lsqlparser -lglib-2.0 -lm  -o ashe
[parallels@Clion-CentOS-7 test]$ ./ashe
table name t1
table name t2
```