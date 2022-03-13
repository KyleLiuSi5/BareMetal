int strncmp(const char* s1, const char* s2, int n)
{
	int count = 0;
	if (*s1 == '\0' && *s2 != 0)
	{
		return (*s1 - *s2);
	}
	if (*s2 == '\0' && *s1 != 0)
	{
		return (*s1 - *s2);
	}
	for (; *s1 != '\0' && *s2 != '\0' &&count<n; s1++, s2++,count++)
	{
		if ((*s1 - *s2) > 0)
		{
			return (*s1 - *s2);
		}
		else if ((*s1 - *s2) <0)
		{
			return (*s1 - *s2);
		}
	}
	return 0;
}