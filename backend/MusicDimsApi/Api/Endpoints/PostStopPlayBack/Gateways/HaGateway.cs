using System.Text.Json;
using System.Text.Json.Serialization;
using MusicDimsApi.Clients;

namespace MusicDimsApi.Endpoints.PostStopPlayBack.Gateways;

public class HaGateway(IHttpClientFactory clientFactory, ILogger<HaGateway> logger) : IHaGateway
{
    private const string GetLibraryRoute = "/api/services/media_player/media_stop";

    public async Task StopPlayback(string id)
    {
        var body = new GetLibraryPostBody($"media_player.{id}");
        var client = clientFactory.CreateClient(HomeAssistantClient.Name);
        var response = await client.PostAsync(GetLibraryRoute, new StringContent(JsonSerializer.Serialize(body), System.Text.Encoding.UTF8, "application/json"));

        var test = await response.Content.ReadAsStringAsync();
        response.EnsureSuccessStatusCode();
    }
}

public record GetLibraryPostBody([property: JsonPropertyName("entity_id")]string EntityId);
public interface IHaGateway
{
    Task StopPlayback(string id);
}