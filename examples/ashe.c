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

	GString* sql_update = g_string_new("update t1 set id =10");
	g_string_append_c(sql_update, '\0'); /* 2 more NULL for lexer EOB */
	g_string_append_c(sql_update, '\0');
	print_sql_table_names(sql_update);
	g_string_free(sql_update,TRUE);

	GString* sql_delete = g_string_new("delete  from t1 where  id =10");
	g_string_append_c(sql_delete, '\0'); /* 2 more NULL for lexer EOB */
	g_string_append_c(sql_delete, '\0');
	print_sql_table_names(sql_delete);
	g_string_free(sql_delete,TRUE);

	GString* sql_insert = g_string_new("insert into t1 values(1,'aa')");
	g_string_append_c(sql_insert, '\0'); /* 2 more NULL for lexer EOB */
	g_string_append_c(sql_insert, '\0');
	print_sql_table_names(sql_insert);
	g_string_free(sql_insert,TRUE);

	GString* sql_ignore = g_string_new("select * from (select * from t1 limit 1) as c");
	g_string_append_c(sql_ignore, '\0'); /* 2 more NULL for lexer EOB */
	g_string_append_c(sql_ignore, '\0');
	print_sql_table_names(sql_ignore);
	g_string_free(sql_ignore,TRUE);

	return 0;
}
