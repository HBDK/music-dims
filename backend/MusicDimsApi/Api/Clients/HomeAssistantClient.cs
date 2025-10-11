using System.Net.Http.Headers;
using MusicDimsApi.Clients.Models;

namespace MusicDimsApi.Clients;

public static class HomeAssistantClient
{
    public const string Name = "HA";

    public static WebApplicationBuilder AddHomeAssistantClient(this WebApplicationBuilder builder)
    {
        var config = builder.Configuration.GetSection("Clients:HA").Get<HomeAssistantConfig>() ?? throw new Exception("Expected to be able to get ");
        
        builder.Services.AddHttpClient(Name, client =>
        {
            client.BaseAddress = new Uri(config.BaseAddress);
            client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", config.Token);
        });
        return builder;
    }
}