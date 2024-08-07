void sortByPrice(vector<service>& services)
{
    for (int i = 0; i < services.size() - 1; ++i)
    {
        for (int j = 0; j < services.size() - i - 1; ++j)
        {
            if (services[j].getPrice() > services[j + 1].getPrice())
            {
                service temp = services[j];
                services[j] = services[j + 1];
                services[j + 1] = temp;
            }
        }
    }
}

void sortByTime(vector<service>& services)
{
    for (int i = 0; i < services.size() - 1; ++i)
    {
        for (int j = 0; j < services.size() - i - 1; ++j)
        {
            if (services[j].getDuration() > services[j + 1].getDuration())
            {
                service temp = services[j];
                services[j] = services[j + 1];
                services[j + 1] = temp;
            }
        }
    }
}

void sortByName(vector<service>& services)
{
    for (int i = 0; i < services.size() - 1; ++i)
    {
        for (int j = 0; j < services.size() - i - 1; ++j)
        {
            if (services[j].getName() > services[j + 1].getName())
            {
                service temp = services[j];
                services[j] = services[j + 1];
                services[j + 1] = temp;
            }
        }
    }
}
