
void merge (int m, const int *sorted_sublist1, const int *sorted_sublist2, int *merged_list)
{
  int i,j,k;
  i = 0;
  j = 0;
  k = 0;

  while (i<m && j<m)
  {
    //printf("list1[%d]: %d    list2[%d]: %d    \n", i, sorted_sublist1[i], j, sorted_sublist1[j]);
    if (sorted_sublist1[i] <=sorted_sublist2[j])
    {
      merged_list[k] = sorted_sublist1[i];
      i++;
    }

    else
    {
      merged_list[k] = sorted_sublist2[j];
      j++;
    }


    k++;
  }

  // my solution
  /*
  while (k<2*m)
  {
    if (i<m)
    {
      merged_list[k] = sorted_sublist1[i];
      i++;
    }

    else
    {
      merged_list[k] = sorted_sublist2[j];
      j++;
    }
    k++;
  }
  */

  // Xing's solution
  while (i<m) {
    merged_list[k++] = sorted_sublist1[i++];
  }

  while (j<m) {
    merged_list[k++] = sorted_sublist2[j++];
  }

}
