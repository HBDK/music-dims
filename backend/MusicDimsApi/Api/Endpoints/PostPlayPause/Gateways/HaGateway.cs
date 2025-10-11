using System.Text.Json;
using System.Text.Json.Serialization;
using MusicDimsApi.Clients;

namespace MusicDimsApi.Endpoints.PostPlayPause.Gateways;

public class HaGateway(IHttpClientFactory clientFactory, ILogger<HaGateway> logger) : IHaGateway
{
    private const string PlayPauseRoute = "/api/services/media_player/media_play_pause";

    public async Task PlayPause(string id)
    {
        var body = new PlayPausePostBody($"media_player.{id}");
        var client = clientFactory.CreateClient(HomeAssistantClient.Name);
        var response = await client.PostAsync(PlayPauseRoute, new StringContent(JsonSerializer.Serialize(body), System.Text.Encoding.UTF8, "application/json"));

        var test = await response.Content.ReadAsStringAsync();
        response.EnsureSuccessStatusCode();
    }
}

public record PlayPausePostBody([property: JsonPropertyName("entity_id")]string EntityId);
public interface IHaGateway
{
    Task PlayPause(string id);
}