using System.Text.Json;
using System.Text.Json.Serialization;
using MusicDimsApi.Clients;
using MusicDimsApi.Endpoints.GetPlayer.Models;

namespace MusicDimsApi.Endpoints.GetPlayer.Gateways;

public class HaGateway(IHttpClientFactory clientFactory, ILogger<HaGateway> logger) : IHaGateway
{
    private string GetStateRoute(string id) => $"/api/states/media_player.{id}";

    public async Task<HaPlayerStateResponse> GetState(string id)
    {
        var route = GetStateRoute(id);
        var client = clientFactory.CreateClient(HomeAssistantClient.Name);
        var response = await client.GetAsync(route);

        response.EnsureSuccessStatusCode();

        var result = await response.Content.ReadFromJsonAsync<HaPlayerStateResponse>() ?? throw new Exception();
        return result;
    }
}

public record VolumeUpPostBody([property: JsonPropertyName("entity_id")]string EntityId);
public interface IHaGateway
{
    Task<HaPlayerStateResponse> GetState(string id);
}