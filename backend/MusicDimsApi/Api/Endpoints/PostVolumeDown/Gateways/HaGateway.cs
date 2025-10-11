using System.Text.Json;
using System.Text.Json.Serialization;
using MusicDimsApi.Clients;

namespace MusicDimsApi.Endpoints.PostVolumeDown.Gateways;

public class HaGateway(IHttpClientFactory clientFactory, ILogger<HaGateway> logger) : IHaGateway
{
    private const string VolumeDownRoute = "/api/services/media_player/volume_down";

    public async Task VolumeDown(string id)
    {
        var body = new VolumeDownPostBody($"media_player.{id}");
        var client = clientFactory.CreateClient(HomeAssistantClient.Name);
        var response = await client.PostAsync(VolumeDownRoute, new StringContent(JsonSerializer.Serialize(body), System.Text.Encoding.UTF8, "application/json"));

        var test = await response.Content.ReadAsStringAsync();
        response.EnsureSuccessStatusCode();
    }
}

public record VolumeDownPostBody([property: JsonPropertyName("entity_id")]string EntityId);
public interface IHaGateway
{
    Task VolumeDown(string id);
}