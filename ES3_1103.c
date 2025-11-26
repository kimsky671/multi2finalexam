//20251404 ±Ë«œ¥√

int ES3action(int hp, char* name)
{
    if (hp == 100) 
    {
        return Heal(hp, name);
    }
    else 
    {
        return Wait(hp, name);

    }
}